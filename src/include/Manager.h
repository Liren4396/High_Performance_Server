// Manager.h
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <shared_mutex>
#include <functional>

class Connection; // 前向声明，避免头文件循环依赖

class Manager {
public:
    static Manager& getInstance();
    // 线程安全的快照
    std::vector<int> getFdsSnapshot();
    // 注册/移除连接
    void append(int fd, Connection* conn);
    void setHash(int fd, std::string name);
    void remove(int fd);
    // 按 fd 获取 Connection 指针（只用于内部跨线程调度场景）
    Connection* getConnection(int fd);
    // 高效的遍历接口：对每个连接执行回调，减少锁的获取次数
    void forEachConnection(std::function<void(int fd, Connection*)> callback);
    // 广播专用接口：对除指定fd外的所有连接执行回调
    void broadcast(int excludeFd, std::function<void(Connection*)> callback);
private:
    std::vector<int> fds;
    std::unordered_map<int, Connection*> fdToConn;
    std::shared_mutex mtx;  // 读写锁：允许多个读操作并发，写操作互斥
    Manager();
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
};
