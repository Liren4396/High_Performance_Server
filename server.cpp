#include "socket.h"
#include "Epoll.h"

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

    Socket* svr = new Socket();
    InetAddress* addr = new InetAddress();
    svr->sbind(addr);
    svr->slisten();

    Epoll* epoll = new Epoll();
    epoll->add(EPOLLIN, svr->getFd());

    while (true) {
        vector<epoll_event> events = epoll->poll();
        int nfds= events.size();
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == svr->getFd()) {            // 发生事件是服务器的fd，表示新客户端连接
                InetAddress* client_addr = new InetAddress();   // 会有内存泄漏, 并且无法delete
                int cliFd = svr->saccept(client_addr);          // 会有内存泄漏, 并且无法delete
                Socket* cli = new Socket(cliFd); 
                
                cli->setnonblocking();
                epoll->add(EPOLLIN | EPOLLET, cli->getFd());
                printf("new client fd %d! IP: %s Port: %d\n", cli->getFd(), inet_ntoa(client_addr->getSockAddress().sin_addr), ntohs(client_addr->getSockAddress().sin_port));
            } else if (events[i].events & EPOLLIN) {
                handleEvent(events[i].data.fd);
            } else {
                cout << "something else happen" << endl;
            }
        }
    }
    delete svr;
    delete addr;
}
