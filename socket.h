#include "InetAddress.h"

#define SOMAXCONN 128

class Socket {
public:
    Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        errif(sockfd == -1, "create sockfd error");
    }

    ~Socket() {

    }

    Socket(int fd) : sockfd(fd) {
        errif(fd == -1, "create socket error");
    }

    void sbind(InetAddress* inetAddress) {
        struct sockaddr_in serv_addr = inetAddress->getSockAddress();
        errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    }

    void slisten() {
        errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
    }

    int getFd() {
        return sockfd;
    }

    int saccept(InetAddress* InetAddr) {
        struct sockaddr_in addr = InetAddr->getSockAddress();
        socklen_t addr_len = sizeof(addr);
        int client_sockfd = accept(sockfd, (sockaddr*)&addr, &addr_len);
        errif(client_sockfd == -1, "accept socket error");
        return client_sockfd;
    }

    void setnonblocking() {
        int flags = fcntl(sockfd, F_GETFL);
        fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    }

private:
    int sockfd;

};