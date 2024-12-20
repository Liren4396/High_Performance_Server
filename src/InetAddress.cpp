#include <string.h>
#include "include/InetAddress.h"
#include "include/Config.h"
InetAddress::InetAddress() {
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    serv_addr.sin_port = htons(PORT);
}

struct sockaddr_in InetAddress::getSockAddress() {
    return serv_addr; 
}

InetAddress::~InetAddress() {}

void InetAddress::setInetAddr(sockaddr_in _addr) {
    serv_addr = _addr;
}