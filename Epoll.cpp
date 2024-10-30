#include "Epoll.h"
#include "Channel.h"
#include "Util.h"

void Epoll::updateChannel(Channel* channel) {
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();

    if (!channel->getInEpoll()) {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
    } else {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }

    delete[] events;
}

std::vector<Channel*> Epoll::poll() {
    std::vector<Channel*> active_event;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    errif(nfds == -1, "epoll wait error");

    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        active_event.push_back(ch);
    }
    return active_event;
}

void Epoll::add(uint32_t op, int fd) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = op;
    ev.data.fd = fd;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

Epoll::Epoll() {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}