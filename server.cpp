#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>

#include "util.h"

#define SOMAXCONN 128
#define MAX_EVENTS 1024

void handleEvent(int sockfd) {
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            cout << "message from client" << sockfd << ": " << buf << endl;
            write(sockfd, buf, sizeof(buf));
        } else if (read_bytes == -1 && errno == EINTR) {
            cout << "continue reading" << endl;
            break;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            cout << "finish reading once, errno: " << errno << endl;
            break;
        } else if (read_bytes == 0) {
            cout << "client" << sockfd << " disconnected" << endl;
            close(sockfd);
            break;
        }
    }
}

void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create sockfd error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    serv_addr.sin_port = htons(PORT);
    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");


    int epfd = epoll_create1(0);
    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) { // 发生事件是服务器的fd，表示新客户端连接
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));
                int client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
                errif(client_sockfd == -1, "accept socket error");
                ev.data.fd = client_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev); 
                printf("new client fd %d! IP: %s Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            } else if (events[i].events & EPOLLIN) {
                handleEvent(events[i].data.fd);
            } else {
                cout << "something else happen" << endl;
            }
        } 

    }
}
