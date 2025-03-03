// Connection.cpp
#include "include/Connection.h"
#include "include/Socket.h"
#include "include/Channel.h"
#include "include/Server.h"
#include "include/Buffer.h"
#include "include/Util.h"
#include "include/Manager.h"
#include "include/MysqlManager.h"

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <mysql/mysql.h>

#define READ_BUFFER 1024

void executeSql(MYSQL* conn, const char* sql) {
    if (mysql_query(conn, sql)) {
        std::cerr << "mysql_query() failed: " << mysql_error(conn) << std::endl;
        return;
    }
}

Connection::Connection(EventLoop* _loop, Socket* _sock) : loop(_loop), sock(_sock), channel(nullptr) {
    if (sock!= nullptr && sock->getFd() >= 0 && _loop!= nullptr) {
        channel = new Channel(loop, sock->getFd());
        if (channel!= nullptr) {
            channel->enableReading();
            channel->useET();
            std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
            channel->SetReadCallback(cb);
        } else {
            std::cerr << "Failed to create Channel for the new connection" << std::endl;
            // 可以在这里添加合适的错误处理逻辑，比如关闭Socket等资源，避免资源泄漏
            if (sock!= nullptr) {
                sock->~Socket();
            }
        }
    } else {
        std::cerr << "Invalid Socket or EventLoop for new connection" << std::endl;
    }
    readBuffer = new Buffer();
    //mysql_conn = MySQLManager::getInstance().getConnection();
    //if (mysql_conn == NULL) {
    //    std::cerr << "Failed to get MySQL connection in Connection" << std::endl;
    //}
}

Connection::~Connection() {
    if (channel!= nullptr) {
        channel->~Channel();
        delete channel;
    }
    if (sock!= nullptr) {
        sock->~Socket();
        delete sock;
    }
    if (readBuffer!= nullptr) {
        delete readBuffer;
    }
}

void Connection::echo(int sockfd) {
    char buf[1024];
    int flag = 1;
    while (true) {
        memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (buf[0] == 25) continue;
        if (read_bytes > 0) {
            size_t start = 0;
            for (size_t i = 0; i < read_bytes; ++i) {
                // if (buf[i] == '\0') break;
                //if (buf[i] == 25) return;
                if (buf[i] == '\3') {
                    // 提取从start到i位置（不包含i，因为i位置是'\3'分隔符）的消息内容
                    std::string message(buf, i);
                    readBuffer->setName(message);
                    //updateNameInDB(sockfd, message);
                    // 处理完这条消息后，更新start位置，准备查找下一个消息
                    start = i + 1;
                    break;
                }
            }
            readBuffer->append(buf+start, read_bytes);
            //if (flag == 1) {
            //    insertToHistoryDB(readBuffer->getName(), readBuffer->getBuffer());
            //    flag = 0;
           // }
        } else if (read_bytes == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            break;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "message from " << readBuffer->getName() << ": " << readBuffer->getBuffer() << std::endl;
            //errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            std::vector<int> list = Manager::getInstance().getFds();
            int if_recorded_inDB = 0;
            for (int fd : list) {
                if (fd != sockfd) {
                    send(fd, readBuffer->getName());
                }
            }
            readBuffer->clear();
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            
            //deleteCurrentVisitorFromDB(sockfd);
            Manager::getInstance().remove(sockfd);
            deleteConnectionCallback(sockfd);
            break;
        } else {
            std::cout << "Connection reset by peer" << std::endl;
            deleteConnectionCallback(sockfd);          //会有bug，注释后单线程无bug
            break;
        }
    }
}

void Connection::insertToHistoryDB(const std::string& name, const std::string& sentence) {
    if (mysql_conn!= NULL) {
        char sql[256];
        sprintf(sql, "INSERT INTO history (sentence, name) VALUES ('%s', '%s')", sentence.c_str(), name.c_str());
        if (mysql_query(mysql_conn, sql)) {
            std::cerr << "mysql_query() failed: " << mysql_error(mysql_conn) << std::endl;
            return;
        }
    }
}

void Connection::updateNameInDB(int sockfd, const std::string& name) {
    if (mysql_conn!= NULL) {
        char selectSql[256];
        sprintf(selectSql, "SELECT name FROM current_visitor WHERE fd = %d", sockfd);
        if (mysql_query(mysql_conn, selectSql)) {
            std::cerr << "mysql_query() failed in select for name: " << mysql_error(mysql_conn) << std::endl;
            return;
        }

        MYSQL_RES* result = mysql_store_result(mysql_conn);
        if (result == NULL) {
            std::cerr << "mysql_store_result() failed: " << mysql_error(mysql_conn) << std::endl;
            return;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row!= NULL) {
            std::string currentName = row[0];
            if (currentName.empty()) {
                // 如果当前名字为空字符串，构造UPDATE语句更新名字
                char updateSql[256];
                sprintf(updateSql, "UPDATE current_visitor SET name = '%s' WHERE fd = %d", name.c_str(), sockfd);
                if (mysql_query(mysql_conn, updateSql)) {
                    std::cerr << "mysql_query() failed in update: " << mysql_error(mysql_conn) << std::endl;
                    return;
                }
            }
        }
        mysql_free_result(result);
    }
}

void Connection::insertToVisitorDB(int fd) {
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
        auto now = std::chrono::system_clock::now();
        std::time_t logout_timestamp = std::chrono::system_clock::to_time_t(now);
        char buffer[80];
        struct tm* timeinfo = localtime(&logout_timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        char insertSql[256];
        sprintf(insertSql, "INSERT INTO visitor (login_time, logout_time, name, ip) VALUES ('%s', '%s', '%s', '%s')",
                row[0], buffer, row[1], row[2]);
        if (mysql_query(mysql_conn, insertSql)) {
            std::cerr << "mysql_query() failed in insert: " << mysql_error(mysql_conn) << std::endl;
            return;
        }
    }
}

void Connection::deleteCurrentVisitorFromDB(int fd) {
    if (mysql_conn!= NULL) {
        //insertToVisitorDB(fd);
        char sql[256];
        sprintf(sql, "DELETE FROM current_visitor WHERE fd = %d", fd);
        if (mysql_query(mysql_conn, sql)) {
            std::cerr << "mysql_query() failed: " << mysql_error(mysql_conn) << std::endl;
            return;
        }
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