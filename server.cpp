#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"

void handleEvent(int sockfd) {
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            std::cout << "message from client" << sockfd << ": " << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
        } else if (read_bytes == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            break;
        } else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        } else if (read_bytes == 0) {
            std::cout << "client" << sockfd << " disconnected" << std::endl;
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

    Socket* svr = new Socket();
    InetAddress* addr = new InetAddress();
    svr->sbind(addr);
    svr->slisten();

    Epoll* epoll = new Epoll();

    Channel* svrChannel = new Channel(epoll, svr->getFd());
    svrChannel->enableReading();

    while (true) {
        std::vector<Channel*> events = epoll->poll();
        int nfds= events.size();
        for (int i = 0; i < nfds; ++i) {
            if (events[i]->getFd() == svr->getFd()) {            // 发生事件是服务器的fd，表示新客户端连接
                InetAddress* client_addr = new InetAddress();   // 会有内存泄漏, 并且无法delete
                int cliFd = svr->saccept(client_addr);          // 会有内存泄漏, 并且无法delete
                Socket* cli = new Socket(cliFd); 
                
                cli->setnonblocking();
                Channel* cliChannel = new Channel(epoll, cliFd);
                cliChannel->enableReading();
                printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), inet_ntoa(client_addr->getSockAddress().sin_addr), ntohs(client_addr->getSockAddress().sin_port));
            } else if (events[i]->getRevents() & EPOLLIN) {
                handleEvent(events[i]->getFd());
            } else {
                std::cout << "something else happen" << std::endl;
            }
        }
    }
    delete svr;
    delete addr;
}
