// MysqlManager.h
#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

class MySQLManager {
public:
    static MySQLManager& getInstance();
    MYSQL* getConnection();
    void closeConnection();
    // SQL转义，防止注入攻击
    std::string escapeString(const std::string& str);
    // 非阻塞入队，返回是否成功（队列已满则丢弃并返回 false）
    bool enqueueSql(const std::string& sql);

private:
    MySQLManager();
    ~MySQLManager();
    MySQLManager(const MySQLManager&) = delete;
    MySQLManager& operator=(const MySQLManager&) = delete;

    MYSQL* mysql_conn;  // 存储MySQL连接句柄（主连接，用于同步查询）
    std::vector<MYSQL*> worker_conns;  // 每个 worker 线程的独立连接
    std::string host;    // MySQL服务器主机地址
    std::string user;    // 登录用户名
    std::string password; // 登录密码
    std::string database; // 要使用的数据库名称

    // 异步写入队列
    std::vector<std::thread> workers;  // 多个工作线程并行执行
    std::mutex mtx;
    std::condition_variable cv;
    std::deque<std::string> queue;
    bool stopping = false;
    size_t maxQueueSize = 10000;
    size_t workerCount = 4;  // 默认 4 个 worker 线程并行执行

    void workerLoop(int workerId);
};
