#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "src/include/Util.h"
#include "src/include/Config.h"
#include <mysql/mysql.h>

// 假设errif函数用于错误处理，若实际实现不同需相应调整
// 功能为当条件满足时输出错误信息并退出程序
#define errif(condition, message) \
    if (condition) { \
        std::cerr << message << ": " << strerror(errno) << std::endl; \
        exit(EXIT_FAILURE); \
    }

int main() {
    int sockfd;  // 将sockfd的定义移到这里，使其作用域覆盖后续代码

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create sockfd error");

    // 设置套接字为非阻塞模式
    int flags = fcntl(sockfd, F_GETFL, 0);
    errif(flags == -1, "fcntl F_GETFL error");
    flags |= O_NONBLOCK;
    errif(fcntl(sockfd, F_SETFL, flags) == -1, "fcntl F_SETFL error");

    std::string name;
    std::cout << "请输入你的名字: ";
    std::cin >> name;
    name += '\3';

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    serv_addr.sin_port = htons(PORT);

    // 发起非阻塞连接
    if (connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 && errno!= EINPROGRESS) {
        errif(true, "connect socket error");
    }

    // 创建epoll实例
    int epoll_fd = epoll_create1(0);
    errif(epoll_fd == -1, "epoll_create1 error");

    // 将套接字添加到epoll关注列表，关注其可写和异常事件（用于非阻塞连接后续处理）
    struct epoll_event event;
    event.events = EPOLLOUT | EPOLLERR;
    event.data.fd = sockfd;
    errif(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1, "epoll_ctl add error");

    // 处理非阻塞连接的后续情况
    bool connected = false;
    while (!connected) {
        struct epoll_event events[1];
        int num_events = epoll_wait(epoll_fd, events, 1, -1);
        errif(num_events == -1, "epoll_wait error");

        for (int i = 0; i < num_events; ++i) {
            if (events[i].events & EPOLLOUT) {
                int error = 0;
                socklen_t len = sizeof(error);
                getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
                if (error == 0) {
                    connected = true;
                    // 连接成功后，修改关注事件为可读和异常事件，用于后续数据读写
                    event.events = EPOLLIN | EPOLLERR;
                    errif(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sockfd, &event) == -1, "epoll_ctl mod error");

                    // 连接成功后，尝试连接MySQL数据库并读取history表最后五条记录
                    MYSQL* mysql_conn = mysql_init(NULL);
                    if (mysql_conn == NULL) {
                        errif(true, "mysql_init error");
                    }
                    if (mysql_real_connect(mysql_conn, "localhost", "visitor", "123456", "ChatRoomDB", 0, NULL, 0) == NULL) {
                        std::cerr << "mysql_real_connect error: " << mysql_error(mysql_conn) << std::endl;
                        // 这里可以选择继续尝试连接或者直接退出，目前示例选择直接退出
                        exit(EXIT_FAILURE);
                    }

                    // 构造查询语句，获取history表的最后五条记录，假设history表结构如之前定义，有id、sentence、user_id字段
                    char sql[256];
                    sprintf(sql, "SELECT sentence, name FROM history ORDER BY id DESC LIMIT 5");
                    if (mysql_query(mysql_conn, sql)) {
                        std::cerr << "mysql_query() failed: " << mysql_error(mysql_conn) << std::endl;
                        // 根据实际情况处理查询失败情况，目前示例选择直接退出
                        exit(EXIT_FAILURE);
                    }

                    MYSQL_RES* result = mysql_store_result(mysql_conn);
                    if (result == NULL) {
                        std::cerr << "mysql_store_result() failed: " << mysql_error(mysql_conn) << std::endl;
                        // 根据实际情况处理结果获取失败情况，目前示例选择直接退出
                        exit(EXIT_FAILURE);
                    }

                    // 输出提示信息，表示即将打印历史记录
                    std::cout << "最近五条历史记录如下：" << std::endl;
                    // 解析并输出查询结果
                    MYSQL_ROW row;
                    while ((row = mysql_fetch_row(result))) {
                        std::cout << row[0] << " said by " << row[1] << std::endl;
                    }

                    mysql_free_result(result);
                    mysql_close(mysql_conn);
                } else {
                    errif(true, "connect socket error");
                }
            } else if (events[i].events & EPOLLERR) {
                int error = 0;
                socklen_t len = sizeof(error);
                getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
                errif(error!= 0, "connect socket error");
            }
        }
    }

    fd_set read_fds;
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));

        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;  // 设置超时时间为100毫秒，可根据实际需求调整

        int ret = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
        errif(ret == -1, "select error");

        if (ret == 0) {
            // 超时情况，这里可以什么都不做，直接进入下一次循环继续等待事件
            continue;
        }

        if (FD_ISSET(sockfd, &read_fds)) {
            // 使用epoll等待可读或异常事件（此处代码逻辑基本不变，只是提取出来方便复用）
            struct epoll_event events[1];
            int num_events = epoll_wait(epoll_fd, events, 1, 0);
            errif(num_events == -1, "epoll_wait error");

            for (int i = 0; i < num_events; ++i) {
                if (events[i].events & EPOLLIN) {
                    bzero(buf, sizeof(buf));
                    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
                    if (read_bytes > 0) {
                        std::string received_msg(buf, read_bytes);
                        size_t pos = received_msg.find('\3');
                        if (pos!= std::string::npos) {
                            std::string sender = received_msg.substr(0, pos);
                            std::string message_content = received_msg.substr(pos + 1);
                            std::cout << "message from " << sender << ": " << message_content << std::endl;
                        } else {
                            // 如果没找到分隔符，可能消息格式有问题，这里可以进行相应处理，比如输出提示信息等
                            std::cout << "Received message with incorrect format: " << received_msg << std::endl;
                        }
                    } else if (read_bytes == 0) {
                        std::cout << "server disconnected" << std::endl;
                        close(sockfd);
                        close(epoll_fd);
                        return 0;
                    } else if (errno!= EAGAIN && errno!= EWOULDBLOCK) {
                        // 如果读操作返回 -1且不是因为非阻塞暂时无数据可读的情况，视为真正的读错误进行处理
                        close(sockfd);
                        close(epoll_fd);
                        errif(true, "socket read error");
                    }
                } else if (events[i].events & EPOLLERR) {
                    close(sockfd);
                    close(epoll_fd);
                    errif(true, "socket error");
                }
            }
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            std::string input;
            std::cin >> input;
            if (input == "bye") {
                close(sockfd);
                close(epoll_fd);
                std::cout << "exit" << std::endl;
                return 0;
            }
            if (!input.empty()) {
                strcpy(buf, input.c_str());
                ssize_t write_bytes = write(sockfd, (name + buf).c_str(), sizeof(buf) + name.size());
                bzero(buf, sizeof(buf));
                if (write_bytes == -1 && errno!= EAGAIN && errno!= EWOULDBLOCK) {
                    // 如果写操作返回 -1且不是因为非阻塞暂时不可写的情况，视为真正的写错误进行处理
                    errif(true, "write error");
                }
            }
        }

        // 这里可以添加适当的延时，比如使用usleep(10000); 让程序休眠10毫秒，避免过于频繁地循环检查读写状态消耗过多CPU资源
    }

    return 0;
}