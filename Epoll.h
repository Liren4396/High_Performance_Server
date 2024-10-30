

#define MAX_EVENTS 1024

class Epoll {
public:
    Epoll() {
        epfd = epoll_create1(0);
        errif(epfd == -1, "epoll create error");
        events = new epoll_event[MAX_EVENTS];
        bzero(events, sizeof(epoll_event) * MAX_EVENTS);
    }

    void add(uint32_t op, int fd) {
        struct epoll_event ev;
        bzero(&ev, sizeof(ev));
        ev.events = op;
        ev.data.fd = fd;
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
    }

    void wait() {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");
    }
    vector<epoll_event> poll() {
        vector<epoll_event> active_event;
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");

        for (int i = 0; i < nfds; ++i) {
            active_event.push_back(events[i]);
        }
        return active_event;
    }
    ~Epoll() {
        if (epfd != -1) {
            close(epfd);
            epfd = -1;
        }

        delete[] events;
    }
private:
    int epfd;
    struct epoll_event* events;
};