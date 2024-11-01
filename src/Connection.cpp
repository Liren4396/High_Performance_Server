#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "Server.h"
#include "Buffer.h"
#include "Util.h"

#include <unistd.h>
#include <string.h>
#include <iostream>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _loop, Socket* _sock) : loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();

    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd) {
    char buf[1024];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            readBuffer->append(buf, read_bytes);
        } else if (read_bytes == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            break;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "message from client" << sockfd << ": " << readBuffer->getBuffer() << std::endl;
            //errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            send(sockfd);
            readBuffer->clear();
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            deleteConnectionCallback(sock);
            break;
        } else {
            std::cout << "Connection reset by peer" << std::endl;
            deleteConnectionCallback(sock);          //会有bug，注释后单线程无bug
            break;
        }
    }
}

void Connection::send(int sockfd) {
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int data_size = readBuffer->size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        std::cout << "1sdf" << std::endl;
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}