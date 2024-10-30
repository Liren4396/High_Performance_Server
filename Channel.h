#include <sys/epoll.h>

class Epoll;
class Channel {
public:
    Channel(Epoll* ep, int fd1);
    ~Channel();
    void enableReading();

    int getFd();

    void setInEpoll();

    bool getInEpoll();

    uint32_t getEvents();

    uint32_t getRevents();

    void setRevents(uint32_t r);

private:
    Epoll* epoll;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
};