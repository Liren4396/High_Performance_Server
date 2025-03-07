// Acceptor.cpp
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
    // handle_request();
    //mysql_conn = MySQLManager::getInstance().getConnection();
    //if (mysql_conn == NULL) {
    //    std::cerr << "Failed to get MySQL connection in Acceptor" << std::endl;
    //}
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::insertToDB(int fd, std::string ip, std::time_t timestamp) {
    char sql[256];
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    sprintf(sql, "INSERT INTO current_visitor (login_time, fd, name, ip) VALUES ('%s', '%d', '', '%s')", buffer, fd, ip.c_str());

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
    //handle_request(cliFd);
    Socket* cli = new Socket(cliFd); 
    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    cli->setnonblocking();

    newConnectionCallback(cli);
    Manager::getInstance().append(cli->getFd()); 
    std::string clientIP = inet_ntoa(client_addr->getSockAddress().sin_addr);
    printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), clientIP.c_str(), ntohs(client_addr->getSockAddress().sin_port));
    // handle_request(cliFd);
    //insertToDB(cli->getFd(), clientIP, timestamp);
    //delete client_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _function) {
    newConnectionCallback = _function;
}

void Acceptor::handle_request(int client_socket) {
    char buffer[1024] = {0};
    // 接收客户端发送的请求数据
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Error reading from client");
        return;
    }
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += "<html><body><h1>Hello from HTTP Server!</h1></body></html>";

    // 发送响应数据给客户端
    if (write(client_socket, response.c_str(), response.length()) < 0) {
        perror("Error writing to client");
    }
    close(client_socket);
}