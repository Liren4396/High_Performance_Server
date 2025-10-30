// Connection.cpp
#include "include/Connection.h"
#include "include/Socket.h"
#include "include/EventLoop.h"
#include "include/Channel.h"
#include "include/Server.h"
#include "include/BufferPool.h"
#include "include/Util.h"
#include "include/Manager.h"
#include "include/MysqlManager.h"

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <mysql/mysql.h>
#include <cerrno>

#define READ_BUFFER 1024

void executeSql(MYSQL* conn, const char* sql) {
    if (mysql_query(conn, sql)) {
        std::cerr << "mysql_query() failed: " << mysql_error(conn) << std::endl;
        return;
    }
}

Connection::Connection(EventLoop* _loop, Socket* _sock) : loop(_loop), sock(_sock), channel(nullptr) {
    if (sock!= nullptr && sock->getFd() >= 0 && _loop!= nullptr) {
        // 优化：如果已经在所属EventLoop线程，直接创建Channel；否则投递
        // 由于Server已经确保在subReactor线程创建Connection，这里通常直接执行
        if (loop->isInLoopThread()) {
            // 已经在所属线程，直接创建（无锁、无系统调用）
            channel = new Channel(loop, sock->getFd());
            if (channel!= nullptr) {
                std::function<void()> readCb = std::bind(&Connection::echo, this, sock->getFd());
                channel->SetReadCallback(readCb);
                std::function<void()> writeCb = std::bind(&Connection::Write, this);
                channel->SetWriteCallback(writeCb);
                channel->useET();
                channel->enableReading();
            }
        } else {
            // 不在所属线程（这种情况应该很少），投递执行
            loop->runInLoop([this]() {
                channel = new Channel(loop, sock->getFd());
                if (channel!= nullptr) {
                    std::function<void()> readCb = std::bind(&Connection::echo, this, sock->getFd());
                    channel->SetReadCallback(readCb);
                    std::function<void()> writeCb = std::bind(&Connection::Write, this);
                    channel->SetWriteCallback(writeCb);
                    channel->useET();
                    channel->enableReading();
                } else {
                    std::cerr << "Failed to create Channel for the new connection" << std::endl;
                }
            });
        }
    } else {
        std::cerr << "Invalid Socket or EventLoop for new connection" << std::endl;
    }
    readBuffer = BufferPool::getInstance().getBuffer();
    writeBuffer = BufferPool::getInstance().getBuffer();
}

Connection::~Connection() {
    if (channel!= nullptr) {
        delete channel;  // 仅 delete，不显式调用析构
        channel = nullptr;
    }
    if (sock!= nullptr) {
        delete sock;     // 仅 delete，不显式调用析构
        sock = nullptr;
    }
    if (readBuffer!= nullptr) {
        BufferPool::getInstance().returnBuffer(readBuffer);
    }
    if (writeBuffer!= nullptr) {
        BufferPool::getInstance().returnBuffer(writeBuffer);
    }
}

void Connection::echo(int sockfd) {
    char buf[4096];
    while (true) {
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            size_t start = 0;
            for (ssize_t i = 0; i < read_bytes; ++i) {
                if (buf[i] == '\3') {
                    std::string message(buf, i);
                    readBuffer->setName(message);
                    start = i + 1;
                    break;
                }
            }
            if (start < static_cast<size_t>(read_bytes)) {
                readBuffer->append(buf + start, read_bytes - start);
            }
            // 继续循环直到读到 EAGAIN
            continue;
        }
        if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            Manager::getInstance().remove(sockfd);
            deleteConnectionCallback(sockfd);
            return;
        }
        if (read_bytes == -1) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cout << "message from " << readBuffer->getName() << ": " << readBuffer->getBuffer() << std::endl;
            // 使用高效的广播接口：一次加锁完成所有操作
            Manager::getInstance().broadcast(sockfd, [this](Connection* peer) {
                peer->postSend(readBuffer->getName(), readBuffer->getBuffer());
            });
                readBuffer->clear();
                return;
            }
            // ECONNRESET(104) 是正常的，表示连接被重置（客户端关闭）
            if (errno == ECONNRESET || errno == 104) {
                std::cout << "client" << sockfd << " disconnected (connection reset)" << std::endl;
            } else {
                std::cerr << "Connection error on read, errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
            }
            Manager::getInstance().remove(sockfd);
            deleteConnectionCallback(sockfd);
            return;
        }
    }
}

void Connection::insertToHistoryDB(const std::string& name, const std::string& sentence) {
    // 转义用户输入，防止SQL注入
    std::string escaped_name = MySQLManager::getInstance().escapeString(name);
    std::string escaped_sentence = MySQLManager::getInstance().escapeString(sentence);
    
    char sql[512];
    snprintf(sql, sizeof(sql), "INSERT INTO history (sentence, name) VALUES ('%s', '%s')", 
             escaped_sentence.c_str(), escaped_name.c_str());
    
    // 使用异步队列，避免阻塞 IO 线程
    if (!MySQLManager::getInstance().enqueueSql(sql)) {
        std::cerr << "Failed to enqueue SQL (queue full): " << sql << std::endl;
    }
}

void Connection::updateNameInDB(int sockfd, const std::string& name) {
    // 转义用户输入，防止SQL注入
    std::string escaped_name = MySQLManager::getInstance().escapeString(name);
    
    // 简化：直接更新名字（如果为空则更新），避免 SELECT 查询
    char updateSql[256];
    snprintf(updateSql, sizeof(updateSql), "UPDATE current_visitor SET name = '%s' WHERE fd = %d AND (name = '' OR name IS NULL)", 
             escaped_name.c_str(), sockfd);
    
    // 使用异步队列，避免阻塞 IO 线程
    if (!MySQLManager::getInstance().enqueueSql(updateSql)) {
        std::cerr << "Failed to enqueue SQL (queue full): " << updateSql << std::endl;
    }
}

void Connection::insertToVisitorDB(int fd) {
    // 注意：此函数仍需要同步 SELECT（需要结果），但 INSERT 部分改为异步
    MYSQL* mysql_conn = MySQLManager::getInstance().getConnection();
    if (mysql_conn == NULL) return;
    
    char selectSql[256];
    sprintf(selectSql, "SELECT login_time, name, ip FROM current_visitor WHERE fd = %d", fd);
    if (mysql_query(mysql_conn, selectSql)) {
        std::cerr << "mysql_query() failed in select: " << mysql_error(mysql_conn) << std::endl;
        return;
    }
    MYSQL_RES* result = mysql_store_result(mysql_conn);
    if (result == NULL) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(mysql_conn) << std::endl;
        return;
    }
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row!= NULL) {
        // 转义从数据库读取的数据，确保安全（虽然理论上数据库数据是安全的）
        std::string escaped_login_time = MySQLManager::getInstance().escapeString(row[0] ? row[0] : "");
        std::string escaped_name = MySQLManager::getInstance().escapeString(row[1] ? row[1] : "");
        std::string escaped_ip = MySQLManager::getInstance().escapeString(row[2] ? row[2] : "");
        
        auto now = std::chrono::system_clock::now();
        std::time_t logout_timestamp = std::chrono::system_clock::to_time_t(now);
        char buffer[80];
        struct tm* timeinfo = localtime(&logout_timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        std::string escaped_logout_time = MySQLManager::getInstance().escapeString(buffer);
        
        char insertSql[512];
        snprintf(insertSql, sizeof(insertSql), "INSERT INTO visitor (login_time, logout_time, name, ip) VALUES ('%s', '%s', '%s', '%s')",
                escaped_login_time.c_str(), escaped_logout_time.c_str(), escaped_name.c_str(), escaped_ip.c_str());
        
        // INSERT 使用异步队列，避免阻塞 IO 线程
        if (!MySQLManager::getInstance().enqueueSql(insertSql)) {
            std::cerr << "Failed to enqueue SQL (queue full): " << insertSql << std::endl;
        }
    }
    mysql_free_result(result);
}

void Connection::deleteCurrentVisitorFromDB(int fd) {
    char sql[256];
    sprintf(sql, "DELETE FROM current_visitor WHERE fd = %d", fd);
    
    // 使用异步队列，避免阻塞 IO 线程
    if (!MySQLManager::getInstance().enqueueSql(sql)) {
        std::cerr << "Failed to enqueue SQL (queue full): " << sql << std::endl;
    }
}

void Connection::send(int sockfd, std::string name) {
    int total_size = name.size() + 1 + readBuffer->size();
    char buf[total_size];
    strcpy(buf, name.c_str());
    buf[name.size()] = '\3';
    strcpy(buf + name.size() + 1, readBuffer->c_str());

    int data_size = total_size;
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> const &_cb) {
    deleteConnectionCallback = _cb;
}

void Connection::postSend(const std::string& name, const std::string& payload) {
    // 在所属 EventLoop 线程中执行实际写入，避免跨线程写
    loop->queueInLoop([this, name, payload]() {
        if (writeBuffer == nullptr) return;
        
        // 构造要发送的数据（优化：预分配容量，减少内存重分配）
        std::string out;
        out.reserve(name.size() + 1 + payload.size());  // 预分配空间
        out.append(name);
        out.push_back('\3');
        out.append(payload);
        
        // 如果写缓冲区已有数据，直接追加
        if (writeBuffer->size() > 0) {
            writeBuffer->append(out.data(), static_cast<int>(out.size()));
            // 已注册EPOLLOUT，等待可写事件
            return;
        }
        
        // 尝试直接发送
        const char* data = out.data();
        size_t total = out.size();
        size_t sent = 0;
        
        while (sent < total) {
            ssize_t n = write(sock->getFd(), data + sent, total - sent);
            if (n == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 无法继续写，放入缓冲区并注册EPOLLOUT
                    writeBuffer->append(out.data() + sent, static_cast<int>(total - sent));
                    if (channel != nullptr) {
                        std::function<void()> writeCb = std::bind(&Connection::Write, this);
                        channel->SetWriteCallback(writeCb);
                        channel->enableWriting();
                    }
                    return;
                }
                // 其他错误，关闭连接
                deleteConnectionCallback(sock->getFd());
                return;
            }
            sent += static_cast<size_t>(n);
        }
        // 全部发送完成
    });
}

void Connection::Write() {
    if (writeBuffer == nullptr || writeBuffer->size() == 0) {
        // 没有数据要写，应该取消EPOLLOUT，但保留EPOLLIN
        // 简化处理：后续可以优化事件管理
        return;
    }
    
    std::string data = writeBuffer->getBuffer();  // 获取副本
    size_t total = data.size();
    size_t sent = 0;
    
    while (sent < total) {
        ssize_t n = write(sock->getFd(), data.data() + sent, total - sent);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 仍然无法写，保留剩余数据，保持EPOLLOUT注册
                if (sent > 0) {
                    writeBuffer->SetBuf(data.substr(sent));  // 保留未发送部分
                }
                return;
            }
            // 其他错误，关闭连接
            deleteConnectionCallback(sock->getFd());
            return;
        }
        sent += static_cast<size_t>(n);
    }
    
    // 全部发送完成，清空缓冲区
    writeBuffer->clear();
    // 注意：这里应该只取消EPOLLOUT，保留EPOLLIN
    // 简化处理：下次发送时会重新注册EPOLLOUT
}