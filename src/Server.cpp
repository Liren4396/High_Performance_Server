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
#include "include/Manager.h"


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
    if (serv_sock->getFd() == -1) return;
    
    int fd = serv_sock->getFd();
    int random = fd % subReactors.size();
    EventLoop* subLoop = subReactors[random];
    
    // 优化：将Connection的创建投递到subReactor线程执行
    // 这样Channel的注册就不需要跨线程，减少锁竞争和系统调用
    subLoop->runInLoop([this, serv_sock, fd, subLoop]() {
        Connection *conn = new Connection(subLoop, serv_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        
        // 在subReactor线程中注册
        {
            std::lock_guard<std::mutex> guard(mutex_);
            connections[fd] = conn;
        }
        Manager::getInstance().append(fd, conn);
    });
}

void Server::deleteConnection(int sock) {
    Connection *conn = nullptr;
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto it = connections.find(sock);
        if (it != connections.end()) {
            conn = it->second;
            connections.erase(sock);
        }
    }
    // 在锁外删除Connection，避免删除时的阻塞操作影响其他连接
    if (conn != nullptr) {
        delete conn;
    }
}

void Server::OnConnect(std::function<void(Connection*)> fn) {
    on_connect_callback_ = fn;
}