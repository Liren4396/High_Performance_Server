// Connection.h
#include <functional>
#include <string>
#include <mysql/mysql.h>
#include <memory>
#include <chrono>
#include <ctime>
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
    void Write();  // 处理EPOLLOUT事件，继续发送写缓冲区中的数据
    void deleteCurrentVisitorFromDB(int fd);
    void updateNameInDB(int sockfd, const std::string& name);
    void insertToVisitorDB(int fd);
    void insertToHistoryDB(const std::string& name, const std::string& sentence);
    // 跨线程安全发送：将写操作投递到所属 EventLoop 执行
    void postSend(const std::string& name, const std::string& payload);
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(int)> deleteConnectionCallback;
    std::string *inBuffer;
    std::shared_ptr<Buffer> readBuffer;
    std::shared_ptr<Buffer> writeBuffer;  // 写缓冲区
    MYSQL* mysql_conn;
};