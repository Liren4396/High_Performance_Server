
#define SOMAXCONN 128
class InetAddress;
class Socket {
public:
    Socket();
    ~Socket();
    Socket(int fd);
    void sbind(InetAddress* inetAddress);
    void slisten();
    int getFd();
    int saccept(InetAddress* InetAddr);
    void setnonblocking();

private:
    int sockfd;

};