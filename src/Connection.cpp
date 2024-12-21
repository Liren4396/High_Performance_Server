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
    channel = new Channel(loop, sock->getFd());
    channel->enableReading();
    channel->useET();
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->SetReadCallback(cb);
    readBuffer = new Buffer();
    mysql_conn = MySQLManager::getInstance().getConnection();
    if (mysql_conn == NULL) {
        std::cerr << "Failed to get MySQL connection in Connection" << std::endl;
    }
}

Connection::~Connection() {
    delete channel;
    delete sock;
    delete readBuffer;
}

void Connection::echo(int sockfd) {
    char buf[1024];
    while (true) {
        memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (buf[0] == 25) continue;
        if (read_bytes > 0) {
            size_t start = 0;
            for (size_t i = 0; i < read_bytes; ++i) {
                if (buf[i] == '\0') break;
                if (buf[i] == '\3') {
                    // 提取从start到i位置（不包含i，因为i位置是'\3'分隔符）的消息内容
                    std::string message(buf, i);
                    readBuffer->setName(message);
                    // 处理完这条消息后，更新start位置，准备查找下一个消息
                    start = i + 1;
                    break;
                }
            }
            readBuffer->append(buf+start, read_bytes);

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
            std::cout << std::endl;
            readBuffer->clear();
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            
            deleteFromDB(sockfd);
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

void Connection::deleteFromDB(int fd) {
    if (mysql_conn!= NULL) {
        // 构造删除对应记录的SQL语句，根据fd删除visitor表中对应的记录，这里假设fd字段唯一标识一条记录
        char sql[256];
        sprintf(sql, "DELETE FROM visitor WHERE fd = %d", fd);
        if (mysql_query(mysql_conn, sql)) {
            std::cerr << "mysql_query() failed: " << mysql_error(mysql_conn) << std::endl;
            // 根据实际情况，这里可以选择更合适的错误处理方式，比如抛出异常或者返回错误码等
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