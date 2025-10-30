// Socket.cpp
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>
#include <iostream>

#include "include/Socket.h"
#include "include/InetAddress.h"
#include "include/Util.h"
Socket::Socket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create sockfd error");
}


Socket::~Socket() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}

Socket::Socket(int fd) : sockfd(fd) {
    errif(fd == -1, "create socket error");
}

void Socket::sbind(InetAddress* inetAddress) {
    struct sockaddr_in serv_addr = inetAddress->getSockAddress();
    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
}

void Socket::slisten() {
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");
}

int Socket::getFd() {
    return sockfd;
}

int Socket::saccept(InetAddress* InetAddr) {
    int clnt_sockfd = -1;
    struct sockaddr_in addr = InetAddr->getSockAddress();
    socklen_t addr_len = sizeof(addr);
    if (fcntl(sockfd, F_GETFL) & O_NONBLOCK) {
        // 非阻塞模式：循环accept直到成功或遇到非EAGAIN错误
        while (true) {
            clnt_sockfd = accept(sockfd, (sockaddr *)&addr, &addr_len);
            if (clnt_sockfd == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 没有更多连接，返回-1表示暂时没有连接
                    return -1;
                } else if (errno == EINTR) {
                    // 被信号中断，继续重试
                    continue;
                } else if (errno == EMFILE || errno == ENFILE) {
                    // 文件描述符耗尽，记录错误但返回-1，让调用者处理
                    std::cerr << "accept failed: too many file descriptors (errno=" << errno << ")" << std::endl;
                    return -1;
                } else if (errno == ECONNABORTED) {
                    // 连接被中止，继续尝试下一个连接
                    continue;
                } else {
                    // 其他错误，记录但返回-1
                    std::cerr << "accept failed: " << strerror(errno) << " (errno=" << errno << ")" << std::endl;
                    return -1;
                }
            } else {
                // accept成功
                break;
            }
        }
    } else {
        // 阻塞模式
        clnt_sockfd = accept(sockfd, (sockaddr *)&addr, &addr_len);
        if (clnt_sockfd == -1) {
            if (errno == EMFILE || errno == ENFILE) {
                std::cerr << "accept failed: too many file descriptors (errno=" << errno << ")" << std::endl;
            } else if (errno != EINTR) {
                std::cerr << "accept failed: " << strerror(errno) << " (errno=" << errno << ")" << std::endl;
            }
            return -1;
        }
    }
    InetAddr->setInetAddr(addr);
    return clnt_sockfd;
}

void Socket::setnonblocking() {
    int flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void Socket::Connect(InetAddress *addr) {
  // for client socket
  struct sockaddr_in tmp_addr = addr->getSockAddress();
  if (fcntl(sockfd, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue;
        /* 连接非阻塞式sockfd建议的做法：
            The socket is nonblocking and the connection cannot be
          completed immediately.  (UNIX domain sockets failed with
          EAGAIN instead.)  It is possible to select(2) or poll(2)
          for completion by selecting the socket for writing.  After
          select(2) indicates writability, use getsockopt(2) to read
          the SO_ERROR option at level SOL_SOCKET to determine
          whether connect() completed successfully (SO_ERROR is
          zero) or unsuccessfully (SO_ERROR is one of the usual
          error codes listed here, explaining the reason for the
          failure).
          这里为了简单、不断连接直到连接完成，相当于阻塞式
        */
      }
      if (ret == -1) {
        errif(true, "socket connect error");
      }
    }
  } else {
    errif(connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
  }
}

void Socket::setFd(int fd) {
  sockfd = fd;
}