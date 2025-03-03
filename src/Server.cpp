// Server.cpp
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <functional>
#include "include/Epoll.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/Config.h"
#include "include/Server.h"
#include "include/Acceptor.h"
#include "include/Connection.h"
#include "include/ThreadPool.h"


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
    std::lock_guard<std::mutex> guard(mutex_);
    if (serv_sock->getFd() != -1) {
        int random = serv_sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[random], serv_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[serv_sock->getFd()] = conn;
        
    }
}

void Server::deleteConnection(int sock) {
    std::lock_guard<std::mutex> guard(mutex_);
    auto it = connections.find(sock);
    if (it != connections.end()) {
        Connection *conn = connections[sock];
        connections.erase(sock);
        delete conn;
    }
}

void Server::OnConnect(std::function<void(Connection*)> fn) {
    on_connect_callback_ = fn;
}