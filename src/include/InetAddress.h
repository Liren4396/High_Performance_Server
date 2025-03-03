// InetAddress.h
#include <arpa/inet.h>
class InetAddress {
public:
    InetAddress();
    struct sockaddr_in getSockAddress();
    ~InetAddress();
    
    void setInetAddr(sockaddr_in _addr);
private:
    struct sockaddr_in serv_addr;
};