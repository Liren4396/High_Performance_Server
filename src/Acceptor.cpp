#include "include/Manager.h"
#include "include/Acceptor.h"
#include "include/Socket.h"
#include "include/InetAddress.h"
#include "include/Channel.h"
#include "include/MysqlManager.h"

Acceptor::Acceptor(EventLoop* _loop) : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
    sock = new Socket();
    InetAddress* addr = new InetAddress();
    sock->sbind(addr);
    sock->slisten();
    sock->setnonblocking();
    Channel* svrChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    svrChannel->SetReadCallback(cb);
    svrChannel->enableReading();
    mysql_conn = MySQLManager::getInstance().getConnection();
    if (mysql_conn == NULL) {
        std::cerr << "Failed to get MySQL connection in Acceptor" << std::endl;
    }
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
    
}

void Acceptor::insertToDB(int fd) {
    char sql[256];
    sprintf(sql, "INSERT INTO current_visitor (fd, name) VALUES (%d, 0)", fd);
    MYSQL* mysql_conn = MySQLManager::getInstance().getConnection();
    if (mysql_conn != NULL) {
        if (mysql_query(mysql_conn, sql)) {
            std::cerr << "mysql_query() failed: " << mysql_error(mysql_conn) << std::endl;
        }
    }
}

void Acceptor::acceptConnection() {
    InetAddress* client_addr = new InetAddress();   // 会有内存泄漏, 并且无法delete
    int cliFd = sock->saccept(client_addr);          // 会有内存泄漏, 并且无法delete
    Socket* cli = new Socket(cliFd); 
    
    cli->setnonblocking();

    newConnectionCallback(cli);
    Manager::getInstance().append(cli->getFd());
    printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), inet_ntoa(client_addr->getSockAddress().sin_addr), ntohs(client_addr->getSockAddress().sin_port));
    insertToDB(cli->getFd());
    delete client_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _function) {
    newConnectionCallback = _function;
}