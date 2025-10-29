// Channel.cpp
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>

#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/Socket.h"
Channel::Channel(EventLoop* _loop, int fd1)
    :loop(_loop), fd(fd1), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::handleEvent() {
  if (revents & (EPOLLIN | EPOLLPRI)) {
    read_callback_();
  }
  if (revents & (EPOLLOUT)) {
    write_callback_();
  }
}

void Channel::enableReading() {
    events |= EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

void Channel::setInEpoll(bool in) { inEpoll = in; }

void Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
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
void Channel::SetReadCallback(std::function<void()> const &callback) { read_callback_ = callback; }
