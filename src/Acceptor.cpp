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
    // 转义IP地址，防止SQL注入
    std::string escaped_ip = MySQLManager::getInstance().escapeString(ip);
    
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    std::string escaped_time = MySQLManager::getInstance().escapeString(buffer);
    
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO current_visitor (login_time, fd, name, ip) VALUES ('%s', %d, '', '%s')", 
             escaped_time.c_str(), fd, escaped_ip.c_str());

    // 使用异步队列，避免阻塞 IO 线程
    if (!MySQLManager::getInstance().enqueueSql(sql)) {
        std::cerr << "Failed to enqueue SQL (queue full): " << sql << std::endl;
    }
}

void Acceptor::acceptConnection() {
    // 在非阻塞模式下，可能需要多次accept来接受所有待处理的连接
    while (true) {
        InetAddress* client_addr = new InetAddress();
        int cliFd = sock->saccept(client_addr);
        
        if (cliFd == -1) {
            delete client_addr;
            // 非阻塞模式下，-1通常表示没有更多连接（EAGAIN）
            // 或者在错误情况下会输出错误信息
            break;
        }
        
        Socket* cli = new Socket(cliFd); 
        auto now = std::chrono::system_clock::now();
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
        cli->setnonblocking();

        // 在回调前先获取IP信息（因为client_addr会在回调后删除）
        std::string clientIP = inet_ntoa(client_addr->getSockAddress().sin_addr);
        int clientPort = ntohs(client_addr->getSockAddress().sin_port);
        
        newConnectionCallback(cli);
        // 由 Server 在创建 Connection 后统一注册到 Manager
        
        printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), clientIP.c_str(), clientPort);
        // handle_request(cliFd);
        //insertToDB(cli->getFd(), clientIP, timestamp);
        
        delete client_addr;  // 使用完毕后释放，修复内存泄漏
    }
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