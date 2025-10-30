// Manager.cpp
#include "include/Manager.h"
#include "include/Connection.h"
#include <mutex>  // for unique_lock

Manager::Manager() = default;

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

std::vector<int> Manager::getFdsSnapshot() {
    std::shared_lock<std::shared_mutex> lock(mtx);  // 读锁，允许多个读操作并发
    return fds; // 拷贝返回
}

void Manager::remove(int fd) {
    std::unique_lock<std::shared_mutex> lock(mtx);  // 写锁，独占访问
    auto it = std::remove(fds.begin(), fds.end(), fd);
    fds.erase(it, fds.end());
    fdToConn.erase(fd);
}

void Manager::append(int fd, Connection* conn) {
    std::unique_lock<std::shared_mutex> lock(mtx);  // 写锁，独占访问
    fds.push_back(fd);
    fdToConn[fd] = conn;
}

Connection* Manager::getConnection(int fd) {
    std::shared_lock<std::shared_mutex> lock(mtx);  // 读锁，允许多个读操作并发
    auto it = fdToConn.find(fd);
    if (it == fdToConn.end()) return nullptr;
    return it->second;
}

void Manager::forEachConnection(std::function<void(int fd, Connection*)> callback) {
    std::shared_lock<std::shared_mutex> lock(mtx);  // 读锁
    // 在锁保护下执行所有回调，避免在回调执行期间连接被删除
    for (auto it = fdToConn.begin(); it != fdToConn.end(); ++it) {
        callback(it->first, it->second);
    }
}

void Manager::broadcast(int excludeFd, std::function<void(Connection*)> callback) {
    // 优化：快速复制连接列表，在锁外执行回调，避免长时间持有锁
    std::vector<Connection*> targets;
    {
        std::shared_lock<std::shared_mutex> lock(mtx);  // 读锁
        targets.reserve(fdToConn.size());  // 预分配空间
        for (auto it = fdToConn.begin(); it != fdToConn.end(); ++it) {
            if (it->first != excludeFd && it->second != nullptr) {
                targets.push_back(it->second);
            }
        }
    }
    // 在锁外执行所有回调，避免阻塞其他操作
    for (Connection* conn : targets) {
        callback(conn);
    }
}