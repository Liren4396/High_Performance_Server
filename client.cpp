#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "util.h"


int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create sockfd error");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    serv_addr.sin_port = htons(PORT);
    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "connect socket error");
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        errif(write_bytes == -1, "write error");

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            cout << "message from srever: " << buf << endl;
        } else if (read_bytes == 0) {
            cout << "server disconnected" << endl;
            close(sockfd);
            break;
        } else {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
}
