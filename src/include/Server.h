// Server.h
#pragma once
#include <map>
#include <vector>
#include <mutex>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
public:
    Server(EventLoop* loop);
    ~Server();

    void handleReadEvent(int sockfd);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sock);
    void OnConnect(std::function<void(Connection*)> fn);
private:
    EventLoop* mainReactor;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool* thpool;
    std::function<void(Connection*)> on_connect_callback_;
    std::mutex mutex_;
};