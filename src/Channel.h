#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel {
public:
    Channel(EventLoop* _loop, int fd1);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();

    void setInEpoll();

    bool getInEpoll();

    uint32_t getEvents();

    uint32_t getRevents();

    void setRevents(uint32_t r);
    void setCallback(std::function<void()> _cb);
private:
    EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
};