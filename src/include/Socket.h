//Socket.h
class InetAddress;
class Socket {
public:
    Socket();
    ~Socket();
    Socket(int fd);
    void sbind(InetAddress* inetAddress);
    void slisten();
    int getFd();
    void setFd(int fd);
    int saccept(InetAddress* InetAddr);
    void setnonblocking();
    void Connect(InetAddress* addr);
private:
    int sockfd;

};