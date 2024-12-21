#include <functional>
#include <string>
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
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;
    Buffer *readBuffer;
};