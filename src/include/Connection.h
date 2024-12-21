#include <functional>
#include <string>
#include <mysql/mysql.h>
class Buffer;
class EventLoop;
class Socket;
class Channel;
class Connection {
public:
    Connection(EventLoop* _loop, Socket* _sock);
    ~Connection();
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)> const &_cb);
    void send(int sockfd, std::string name);
    void Read();
    void Write();
    void deleteFromDB(int fd);
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer *readBuffer;
    MYSQL* mysql_conn;
};