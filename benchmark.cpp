#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <string>
#include <cstring>
#include <climits>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sstream>
#include <iomanip>
#include <errno.h>
#include <cstring>
#include <mutex>
#include "src/include/Config.h"

// 统计信息
struct Stats {
    std::atomic<long long> total_requests{0};      // 总请求数
    std::atomic<long long> success_requests{0};    // 成功请求数
    std::atomic<long long> failed_requests{0};    // 失败请求数
    std::atomic<long long> total_bytes_sent{0};   // 总发送字节数
    std::atomic<long long> total_bytes_recv{0};   // 总接收字节数
    std::atomic<long long> total_latency_us{0};    // 总延迟（微秒）
    std::atomic<long long> min_latency_us{LLONG_MAX}; // 最小延迟
    std::atomic<long long> max_latency_us{0};      // 最大延迟
    
    // 延迟分布统计（单位：微秒）
    std::atomic<long long> latency_0_10{0};   // 0-10ms
    std::atomic<long long> latency_10_50{0};  // 10-50ms
    std::atomic<long long> latency_50_100{0}; // 50-100ms
    std::atomic<long long> latency_100_500{0}; // 100-500ms
    std::atomic<long long> latency_500_plus{0}; // 500ms+
    
    void reset() {
        total_requests = 0;
        success_requests = 0;
        failed_requests = 0;
        total_bytes_sent = 0;
        total_bytes_recv = 0;
        total_latency_us = 0;
        min_latency_us = LLONG_MAX;
        max_latency_us = 0;
        latency_0_10 = 0;
        latency_10_50 = 0;
        latency_50_100 = 0;
        latency_100_500 = 0;
        latency_500_plus = 0;
    }
    
    void printReport(double duration_seconds) {
        long long total = total_requests.load();
        long long success = success_requests.load();
        long long failed = failed_requests.load();
        long long sent = total_bytes_sent.load();
        long long recv = total_bytes_recv.load();
        long long total_lat = total_latency_us.load();
        long long min_lat = min_latency_us.load();
        long long max_lat = max_latency_us.load();
        
        if (total == 0) {
            std::cout << "没有统计数据\n";
            return;
        }
        
        double qps = total / duration_seconds;
        double success_rate = (success * 100.0) / total;
        double avg_latency_ms = (total_lat / 1000.0) / success;
        double min_latency_ms = min_lat == LLONG_MAX ? 0 : min_lat / 1000.0;
        double max_latency_ms = max_lat / 1000.0;
        double throughput_sent = (sent / 1024.0 / 1024.0) / duration_seconds;
        double throughput_recv = (recv / 1024.0 / 1024.0) / duration_seconds;
        
        std::cout << "\n========== 压测报告 ==========\n";
        std::cout << "测试时长: " << std::fixed << std::setprecision(2) << duration_seconds << " 秒\n";
        std::cout << "总请求数: " << total << "\n";
        std::cout << "成功请求: " << success << " (" << std::setprecision(2) << success_rate << "%)\n";
        std::cout << "失败请求: " << failed << " (" << std::setprecision(2) << (100.0 - success_rate) << "%)\n";
        std::cout << "QPS: " << std::setprecision(2) << qps << " requests/sec\n";
        std::cout << "平均延迟: " << std::setprecision(2) << avg_latency_ms << " ms\n";
        std::cout << "最小延迟: " << std::setprecision(2) << min_latency_ms << " ms\n";
        std::cout << "最大延迟: " << std::setprecision(2) << max_latency_ms << " ms\n";
        std::cout << "发送吞吐: " << std::setprecision(2) << throughput_sent << " MB/s\n";
        std::cout << "接收吞吐: " << std::setprecision(2) << throughput_recv << " MB/s\n";
        std::cout << "\n延迟分布:\n";
        std::cout << "  0-10ms:   " << latency_0_10.load() << " (" << (latency_0_10.load() * 100.0 / success) << "%)\n";
        std::cout << "  10-50ms:  " << latency_10_50.load() << " (" << (latency_10_50.load() * 100.0 / success) << "%)\n";
        std::cout << "  50-100ms: " << latency_50_100.load() << " (" << (latency_50_100.load() * 100.0 / success) << "%)\n";
        std::cout << "  100-500ms:" << latency_100_500.load() << " (" << (latency_100_500.load() * 100.0 / success) << "%)\n";
        std::cout << "  500ms+:   " << latency_500_plus.load() << " (" << (latency_500_plus.load() * 100.0 / success) << "%)\n";
        std::cout << "================================\n\n";
    }
};

Stats g_stats;
std::mutex g_print_mutex; // 保护输出，避免多线程混乱

// 设置socket为非阻塞
int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 非阻塞连接，返回错误码（0表示成功，负数表示失败）
int connectNonBlocking(int sockfd, const sockaddr* addr, socklen_t addrlen) {
    if (connect(sockfd, addr, addrlen) == 0) {
        return 0; // 成功
    }
    if (errno == EINPROGRESS) {
        // 使用epoll等待连接完成
        int epfd = epoll_create1(0);
        if (epfd == -1) {
            return -1; // epoll创建失败
        }
        
        epoll_event ev;
        ev.events = EPOLLOUT | EPOLLERR;
        ev.data.fd = sockfd;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
            close(epfd);
            return -1; // epoll_ctl失败
        }
        
        epoll_event events[1];
        int nfds = epoll_wait(epfd, events, 1, 5000); // 5秒超时
        close(epfd);
        
        if (nfds > 0) {
            if (events[0].events & EPOLLERR) {
                int error = 0;
                socklen_t len = sizeof(error);
                if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
                    return -error; // 返回错误码（负数）
                }
                return -1;
            }
            if (events[0].events & EPOLLOUT) {
                int error = 0;
                socklen_t len = sizeof(error);
                if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0) {
                    return 0; // 成功
                } else if (error != 0) {
                    return -error; // 返回错误码
                }
            }
        } else if (nfds == 0) {
            return -ETIMEDOUT; // 超时
        } else {
            return -errno; // epoll_wait失败
        }
    } else {
        return -errno; // 立即失败，返回错误码
    }
    return -1;
}

// 发送消息并接收响应
bool sendAndReceive(int sockfd, const std::string& name, const std::string& message, 
                    long long& latency_us, long long& bytes_sent, long long& bytes_recv) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 构造消息: name + '\3' + message
    std::string msg = name + '\3' + message;
    bytes_sent = msg.size();
    
    // 发送数据
    ssize_t sent = 0;
    const char* data = msg.c_str();
    size_t total = msg.size();
    
    while (sent < total) {
        ssize_t n = write(sockfd, data + sent, total - sent);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 等待可写
                epoll_event ev;
                ev.events = EPOLLOUT;
                ev.data.fd = sockfd;
                int epfd = epoll_create1(0);
                epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
                epoll_event events[1];
                epoll_wait(epfd, events, 1, 1000);
                close(epfd);
                continue;
            }
            return false;
        }
        sent += n;
    }
    
    // 接收响应（简单读取，直到收到分隔符）
    char buf[4096];
    std::string response;
    bool received = false;
    auto recv_start = std::chrono::high_resolution_clock::now();
    
    // 设置5秒接收超时
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - recv_start).count() < 5000) {
        ssize_t n = read(sockfd, buf, sizeof(buf) - 1);
        if (n > 0) {
            response.append(buf, n);
            bytes_recv += n;
            if (response.find('\3') != std::string::npos) {
                received = true;
                break;
            }
        } else if (n == 0) {
            // 连接关闭
            return false;
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            return false;
        } else {
            // 等待数据到达
            epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = sockfd;
            int epfd = epoll_create1(0);
            epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
            epoll_event events[1];
            epoll_wait(epfd, events, 1, 100);
            close(epfd);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    latency_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return received;
}

// 更新延迟分布统计
void updateLatencyDistribution(long long latency_us) {
    long long latency_ms = latency_us / 1000;
    
    if (latency_ms < 10) {
        g_stats.latency_0_10++;
    } else if (latency_ms < 50) {
        g_stats.latency_10_50++;
    } else if (latency_ms < 100) {
        g_stats.latency_50_100++;
    } else if (latency_ms < 500) {
        g_stats.latency_100_500++;
    } else {
        g_stats.latency_500_plus++;
    }
}

// 工作线程：每个线程维持一个连接，持续发送消息
void workerThread(int thread_id, int messages_per_sec, int duration_seconds) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int sockfd = -1;
    
    // 尝试连接服务器，如果失败则重试直到超时
    bool connected = false;
    int retry_count = 0;
    const int max_retries = 60; // 最多重试60次
    const int retry_interval_ms = 1000; // 每次重试间隔1秒
    int last_error = 0;
    bool first_failure = true;
    
    while (!connected && retry_count < max_retries) {
        // 创建socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            last_error = errno;
            if (first_failure) {
                std::lock_guard<std::mutex> lock(g_print_mutex);
                std::cerr << "Thread " << thread_id << ": socket create failed: " << strerror(errno) << std::endl;
                first_failure = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(retry_interval_ms));
            retry_count++;
            continue;
        }
        
        // 设置为非阻塞
        if (setNonBlocking(sockfd) == -1) {
            last_error = errno;
            if (first_failure) {
                std::lock_guard<std::mutex> lock(g_print_mutex);
                std::cerr << "Thread " << thread_id << ": setNonBlocking failed: " << strerror(errno) << std::endl;
                first_failure = false;
            }
            close(sockfd);
            sockfd = -1;
            std::this_thread::sleep_for(std::chrono::milliseconds(retry_interval_ms));
            retry_count++;
            continue;
        }
        
        // 连接服务器
        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ADDRESS);
        serv_addr.sin_port = htons(PORT);
        
        int conn_result = connectNonBlocking(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
        if (conn_result == 0) {
            connected = true;
            break;
        } else {
            last_error = -conn_result; // 转换回正数错误码
            // 只在每10次重试时输出一次，避免输出过多
            if (retry_count % 10 == 0 || retry_count < 3) {
                std::lock_guard<std::mutex> lock(g_print_mutex);
                std::cerr << "Thread " << thread_id << ": connect failed (attempt " << (retry_count + 1) 
                          << "/" << max_retries << ", error: " << strerror(last_error) << ")" << std::endl;
            }
            close(sockfd);
            sockfd = -1;
            
            // 检查是否已经超时
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
            if (duration_seconds > 0 && elapsed >= duration_seconds) {
                // 如果已经超时，直接退出
                return;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(retry_interval_ms));
            retry_count++;
        }
    }
    
    if (!connected) {
        {
            std::lock_guard<std::mutex> lock(g_print_mutex);
            std::cerr << "Thread " << thread_id << ": failed to connect after " << retry_count 
                      << " attempts (last error: " << strerror(last_error) << ")" << std::endl;
        }
        // 即使连接失败，也等待到指定时间再退出
        if (duration_seconds > 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
            if (elapsed < duration_seconds) {
                std::this_thread::sleep_for(std::chrono::seconds(duration_seconds - elapsed));
            }
        }
        return;
    }
    
    // 发送用户名
    std::string name = "benchmark_" + std::to_string(thread_id);
    std::string name_msg = name + '\3';
    ssize_t sent = 0;
    const char* name_data = name_msg.c_str();
    size_t name_len = name_msg.size();
    
    while (sent < name_len) {
        ssize_t n = write(sockfd, name_data + sent, name_len - sent);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            close(sockfd);
            return;
        }
        sent += n;
    }
    
    // 计算发送间隔（微秒）
    int interval_us = messages_per_sec > 0 ? (1000000 / messages_per_sec) : 0;
    
    int message_count = 0;
    
    // 持续发送消息
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        
        if (duration_seconds > 0 && elapsed >= duration_seconds) {
            break;
        }
        
        // 发送消息
        std::string message = "benchmark message #" + std::to_string(message_count++);
        long long latency_us = 0;
        long long bytes_sent = 0;
        long long bytes_recv = 0;
        
        g_stats.total_requests++;
        
        bool success = sendAndReceive(sockfd, name, message, latency_us, bytes_sent, bytes_recv);
        
        if (success) {
            g_stats.success_requests++;
            g_stats.total_bytes_sent += bytes_sent;
            g_stats.total_bytes_recv += bytes_recv;
            g_stats.total_latency_us += latency_us;
            
            // 更新最小延迟
            long long current_min = g_stats.min_latency_us.load();
            while (latency_us < current_min && 
                   !g_stats.min_latency_us.compare_exchange_weak(current_min, latency_us)) {
                current_min = g_stats.min_latency_us.load();
            }
            
            // 更新最大延迟
            long long current_max = g_stats.max_latency_us.load();
            while (latency_us > current_max && 
                   !g_stats.max_latency_us.compare_exchange_weak(current_max, latency_us)) {
                current_max = g_stats.max_latency_us.load();
            }
            
            updateLatencyDistribution(latency_us);
        } else {
            g_stats.failed_requests++;
        }
        
        // 控制发送频率
        if (interval_us > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(interval_us));
        }
    }
    
    close(sockfd);
}

// 实时统计显示线程
void statsThread(int duration_seconds) {
    if (duration_seconds <= 0) return;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    int report_interval = 1; // 每秒报告一次
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(report_interval));
        
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        
        if (duration_seconds > 0 && elapsed >= duration_seconds) {
            break;
        }
        
        long long total = g_stats.total_requests.load();
        long long success = g_stats.success_requests.load();
        long long failed = g_stats.failed_requests.load();
        
        if (total > 0) {
            std::cout << "[" << elapsed << "s] "
                      << "总请求: " << total << ", "
                      << "成功: " << success << ", "
                      << "失败: " << failed << ", "
                      << "QPS: " << (total / (elapsed + 1)) << std::endl;
        }
    }
}

void printUsage(const char* prog_name) {
    std::cout << "用法: " << prog_name << " [选项]\n"
              << "选项:\n"
              << "  -c, --clients <num>        并发客户端数量 (默认: 10)\n"
              << "  -r, --rate <num>          每个客户端每秒发送消息数 (默认: 10)\n"
              << "  -t, --time <seconds>      测试持续时间(秒), 0表示持续运行 (默认: 30)\n"
              << "  -h, --help                显示帮助信息\n"
              << "\n示例:\n"
              << "  " << prog_name << " -c 100 -r 20 -t 60   # 100个客户端，每个每秒20条消息，持续60秒\n"
              << "  " << prog_name << " -c 50 -r 10 -t 0     # 50个客户端，每个每秒10条消息，持续运行\n";
}

int main(int argc, char* argv[]) {
    int num_clients = 10;
    int messages_per_sec = 10;
    int duration_seconds = 30;
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--clients") {
            if (i + 1 < argc) {
                num_clients = std::stoi(argv[++i]);
            } else {
                std::cerr << "错误: -c 需要指定客户端数量\n";
                return 1;
            }
        } else if (arg == "-r" || arg == "--rate") {
            if (i + 1 < argc) {
                messages_per_sec = std::stoi(argv[++i]);
            } else {
                std::cerr << "错误: -r 需要指定消息速率\n";
                return 1;
            }
        } else if (arg == "-t" || arg == "--time") {
            if (i + 1 < argc) {
                duration_seconds = std::stoi(argv[++i]);
            } else {
                std::cerr << "错误: -t 需要指定持续时间\n";
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }
    
    std::cout << "========== Benchmark 压测工具 ==========\n";
    std::cout << "服务器地址: " << ADDRESS << ":" << PORT << "\n";
    std::cout << "并发客户端: " << num_clients << "\n";
    std::cout << "消息速率: " << messages_per_sec << " 消息/秒/客户端\n";
    std::cout << "测试时长: " << (duration_seconds > 0 ? std::to_string(duration_seconds) + " 秒" : "持续运行") << "\n";
    std::cout << "总预期QPS: " << (num_clients * messages_per_sec) << "\n";
    std::cout << "========================================\n\n";
    
    // 预先检查服务器是否可连接
    int test_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (test_sock != -1) {
        setNonBlocking(test_sock);
        sockaddr_in test_addr;
        memset(&test_addr, 0, sizeof(test_addr));
        test_addr.sin_family = AF_INET;
        test_addr.sin_addr.s_addr = inet_addr(ADDRESS);
        test_addr.sin_port = htons(PORT);
        
        int conn_result = connectNonBlocking(test_sock, (sockaddr*)&test_addr, sizeof(test_addr));
        close(test_sock);
        
        if (conn_result != 0) {
            std::cerr << "警告: 无法连接到服务器 " << ADDRESS << ":" << PORT << std::endl;
            std::cerr << "请确保服务器已启动: ./server" << std::endl;
            std::cerr << "压测将继续尝试连接，但可能会失败..." << std::endl << std::endl;
        } else {
            std::cout << "服务器连接检查通过！" << std::endl << std::endl;
        }
    }
    
    // 重置统计信息
    g_stats.reset();
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 启动统计显示线程
    std::thread stats_thread(statsThread, duration_seconds);
    
    // 启动工作线程
    std::vector<std::thread> workers;
    for (int i = 0; i < num_clients; i++) {
        workers.emplace_back(workerThread, i, messages_per_sec, duration_seconds);
    }
    
    // 等待所有工作线程完成
    for (auto& t : workers) {
        t.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time).count() / 1000.0;
    
    // 等待统计线程
    if (stats_thread.joinable()) {
        stats_thread.join();
    }
    
    // 打印最终报告
    g_stats.printReport(duration);
    
    return 0;
}

