// EventLoop.cpp
#include "include/EventLoop.h"
#include "include/Epoll.h"
#include "include/Channel.h"
#include "include/ThreadPool.h"
#include <vector>

EventLoop::EventLoop()
    :epoll(nullptr), quit(false) {
        epoll = new Epoll();
        threadPool = new ThreadPool();
    }

EventLoop::~EventLoop() {
    delete epoll;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = epoll->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) {
    epoll->updateChannel(channel);
}

void EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}