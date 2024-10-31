
#include <arpa/inet.h>
class InetAddress {
public:
    InetAddress();
    struct sockaddr_in getSockAddress();
    ~InetAddress();
private:
    struct sockaddr_in serv_addr;
};