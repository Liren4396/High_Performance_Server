#pragma once
#include <map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop* el);
    ~Server();

    void handleReadEvent(int sockfd);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
private:
    EventLoop* loop;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
};