// EventLoop.h
#include <functional>
#include <vector>
#include <mutex>
#include <thread>

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

    // 线程调度接口
    void runInLoop(const std::function<void()>& cb);
    void queueInLoop(const std::function<void()>& cb);
    bool isInLoopThread() const;
private:
    Epoll *epoll;
    ThreadPool *threadPool;
    bool quit;

    // 唤醒与任务队列
    int wakeupFd;
    Channel* wakeupChannel;
    std::thread::id threadId;
    std::mutex pendingMutex;
    std::vector<std::function<void()>> pendingFunctors;

    void wakeup();
    void handleWakeupRead();
    void doPendingFunctors();
};