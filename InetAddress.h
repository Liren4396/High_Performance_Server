#include "util.h"

class InetAddress {
public:
    InetAddress() {
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
        serv_addr.sin_port = htons(PORT);
    }
    struct sockaddr_in getSockAddress() {
        return serv_addr; 
    }
    ~InetAddress() {

    }
private:
    struct sockaddr_in serv_addr;
};