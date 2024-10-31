class Channel;
class Epoll;
class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel* channel);

private:
    Epoll *epoll;
    bool quit;
};