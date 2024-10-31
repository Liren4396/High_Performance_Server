#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* _loop, int fd1)
    :loop(_loop), fd(fd1), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::handleEvent() {
    callback();
}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

void Channel::setRevents(uint32_t r) {
    revents = r;
}

void Channel::setCallback(std::function<void()> _cb) {
    callback = _cb;
}