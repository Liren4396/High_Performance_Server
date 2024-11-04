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
#include "ThreadPool.h"

Server::Server(EventLoop* loop): mainReactor(loop), acceptor(nullptr) {
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();  // 线程数量，也是subReactor数量
    thpool = new ThreadPool(size);
    for (int i = 0; i < size; ++i) {
        subReactors.push_back(new EventLoop());
    }

    for (int i = 0; i < size; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(sub_loop);
    }
}

Server::~Server() {
    delete acceptor;
    delete thpool;
}

void Server::newConnection(Socket *serv_sock) {
    if (serv_sock->getFd() != -1) {
        int random = serv_sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[random], serv_sock);
        std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[serv_sock->getFd()] = conn;
    }
}

void Server::deleteConnection(Socket* sock) {
    if (sock->getFd() != -1) {
        auto it = connections.find(sock->getFd());
        if (it != connections.end()) {
            Connection *conn = connections[sock->getFd()];
            connections.erase(sock->getFd());
            delete conn;
        }
    }
}

void OnConnect(std::function<void(Connection*)> fn) {
    on_connect_callback_ = fn;
}