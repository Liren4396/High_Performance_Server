#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll* ep, int fd1)
    :epoll(ep), fd(fd1), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    epoll->updateChannel(this);
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