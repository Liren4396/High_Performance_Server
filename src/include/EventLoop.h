// EventLoop.h
#include <functional>

class Channel;
class Epoll;
class ThreadPool;
class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel* channel);
    void addThread(std::function<void()> func);
private:
    Epoll *epoll;
    ThreadPool *threadPool;
    bool quit;
};