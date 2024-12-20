#include "include/Connection.h"
#include "include/Socket.h"
#include "include/Channel.h"
#include "include/Server.h"
#include "include/Buffer.h"
#include "include/Util.h"
#include "include/Manager.h"

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _loop, Socket* _sock) : loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    channel->enableReading();
    channel->useET();
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->SetReadCallback(cb);
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete sock;
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
            for (int fd: list) {
                if (fd != sockfd) {
                    send(fd, readBuffer->getName());
                }
            }
            readBuffer->clear();
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            std::vector<int> list = Manager::getInstance().getFds();
            list.erase(std::remove(list.begin(), list.end(), sockfd), list.end());
            deleteConnectionCallback(sock);
            break;
        } else {
            std::cout << "Connection reset by peer" << std::endl;
            deleteConnectionCallback(sock);          //会有bug，注释后单线程无bug
            break;
        }
    }
}

void Connection::send(int sockfd, std::string name) {
    // 计算拼接后总的数据长度，包括name长度、分隔符长度（这里是1个字符）和原readBuffer内容长度
    int total_size = name.size() + 1 + readBuffer->size();
    char buf[total_size];

    // 先将name复制到buf中
    strcpy(buf, name.c_str());
    // 再添加分隔符，这里用'\3'作为示例分隔符，你可按需替换
    buf[name.size()] = '\3';
    // 最后将readBuffer中的内容复制到buf中
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

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}