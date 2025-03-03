// Channel.h
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

    void setInEpoll(bool in = true);

    bool getInEpoll();

    uint32_t getEvents();
    void useET();
    uint32_t getRevents();

    void setRevents(uint32_t r);
    void SetReadCallback(std::function<void()> const &callback);
private:
    EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
};