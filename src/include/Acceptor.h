#include <functional>
#include <mysql/mysql.h>
#include <string>
#include <chrono>
#include <ctime>

class EventLoop;
class Socket;
class Channel;
class Manager;
class Acceptor {
public:
    Acceptor(EventLoop* _loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)> _function);
    void insertToDB(int fd, std::string ip, std::time_t timestamp);
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
    MYSQL* mysql_conn;
};