#include "include/Acceptor.h"
#include "include/Socket.h"
#include "include/InetAddress.h"
#include "include/Channel.h"

Acceptor::Acceptor(EventLoop* _loop) : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
    sock = new Socket();
    InetAddress* addr = new InetAddress();
    sock->sbind(addr);
    sock->slisten();
    sock->setnonblocking();

    Channel* svrChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    svrChannel->setCallback(cb);
    svrChannel->enableReading();

    delete addr;

}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress* client_addr = new InetAddress();   // 会有内存泄漏, 并且无法delete
    int cliFd = sock->saccept(client_addr);          // 会有内存泄漏, 并且无法delete
    Socket* cli = new Socket(cliFd); 
    
    cli->setnonblocking();

    newConnectionCallback(cli);
    printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), inet_ntoa(client_addr->getSockAddress().sin_addr), ntohs(client_addr->getSockAddress().sin_port));
    delete client_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _function) {
    newConnectionCallback = _function;
}