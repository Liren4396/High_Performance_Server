// Epoll.h
#define MAX_EVENTS 1024

#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
public:
    Epoll();
    void add(uint32_t op, int fd);
    std::vector<Channel*> poll();
    ~Epoll();
    void updateChannel(Channel* channel);
    void DeleteChannel(Channel *ch);
private:
    int epfd;
    struct epoll_event* events;
};