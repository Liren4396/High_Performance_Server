// EventLoop.cpp
#include "include/EventLoop.h"
#include "include/Epoll.h"
#include "include/Channel.h"
#include "include/ThreadPool.h"
#include <vector>
#include <sys/eventfd.h>
#include <unistd.h>
#include "include/Util.h"

EventLoop::EventLoop()
    :epoll(nullptr), quit(false), wakeupFd(-1), wakeupChannel(nullptr) {
        epoll = new Epoll();
        threadPool = new ThreadPool();
        threadId = std::this_thread::get_id();
        wakeupFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        errif(wakeupFd == -1, "eventfd create error");
        wakeupChannel = new Channel(this, wakeupFd);
        wakeupChannel->SetReadCallback(std::bind(&EventLoop::handleWakeupRead, this));
        wakeupChannel->enableReading();
    }

EventLoop::~EventLoop() {
    delete epoll;
    if (wakeupChannel) {
        delete wakeupChannel;
        wakeupChannel = nullptr;
    }
    if (wakeupFd != -1) {
        close(wakeupFd);
        wakeupFd = -1;
    }
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = epoll->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
        doPendingFunctors();
    }
}

void EventLoop::updateChannel(Channel* channel) {
    epoll->updateChannel(channel);
}

void EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}

bool EventLoop::isInLoopThread() const {
    return std::this_thread::get_id() == threadId;
}

void EventLoop::runInLoop(const std::function<void()>& cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const std::function<void()>& cb) {
    {
        std::lock_guard<std::mutex> lock(pendingMutex);
        pendingFunctors.push_back(cb);
    }
    wakeup();
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd, &one, sizeof(one));
    (void)n;
}

void EventLoop::handleWakeupRead() {
    uint64_t one;
    ssize_t n = read(wakeupFd, &one, sizeof(one));
    (void)n;
}

void EventLoop::doPendingFunctors() {
    std::vector<std::function<void()>> functors;
    {
        std::lock_guard<std::mutex> lock(pendingMutex);
        functors.swap(pendingFunctors);
    }
    for (auto &f : functors) {
        f();
    }
}