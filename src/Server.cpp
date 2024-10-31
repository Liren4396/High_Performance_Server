#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <functional>
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Server.h"
#include "Acceptor.h"
#include "Connection.h"


Server::Server(EventLoop* el): loop(el), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::handleReadEvent(int sockfd) {

    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            std::cout << "message from client" << sockfd << ": " << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
        } else if (read_bytes == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            break;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock) {
    Connection *conn = new Connection(loop, serv_sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[serv_sock->getFd()] = conn;
}

void Server::deleteConnection(Socket* sock) {
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}