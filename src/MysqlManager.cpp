// MysqlManager.cpp
#include "include/MysqlManager.h"

MySQLManager::MySQLManager() : mysql_conn(nullptr), host("localhost"), user("root"), password("123456"), database("ChatRoomDB") {
    mysql_conn = mysql_init(NULL);
    if (mysql_conn == NULL) {
        std::cerr << "mysql_init() failed" << std::endl;
        return;
    }

    if (mysql_real_connect(mysql_conn, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(mysql_conn) << std::endl;
        mysql_close(mysql_conn);
        mysql_conn = NULL;
        return;
    }
    if (mysql_set_character_set(mysql_conn, "utf8")!= 0) {
        std::cerr << "mysql_set_character_set() failed: " << mysql_error(mysql_conn) << std::endl;
        mysql_close(mysql_conn);
        mysql_conn = NULL;
        return;
    }
    
    // 为每个 worker 线程创建独立的 MySQL 连接
    for (size_t i = 0; i < workerCount; ++i) {
        MYSQL* conn = mysql_init(NULL);
        if (conn == NULL) {
            std::cerr << "mysql_init() failed for worker " << i << std::endl;
            continue;
        }
        if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0) == NULL) {
            std::cerr << "mysql_real_connect() failed for worker " << i << ": " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            continue;
        }
        if (mysql_set_character_set(conn, "utf8")!= 0) {
            std::cerr << "mysql_set_character_set() failed for worker " << i << std::endl;
            mysql_close(conn);
            continue;
        }
        worker_conns.push_back(conn);
    }
    
    // 启动多个后台工作线程并行执行
    workers.reserve(worker_conns.size());
    for (size_t i = 0; i < worker_conns.size(); ++i) {
        workers.emplace_back(&MySQLManager::workerLoop, this, i);
    }
}

MySQLManager::~MySQLManager() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stopping = true;
    }
    cv.notify_all();
    // 等待所有 worker 线程结束
    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }
    // 关闭所有 worker 连接
    for (MYSQL* conn : worker_conns) {
        if (conn != NULL) {
            mysql_close(conn);
        }
    }
    // 关闭主连接
    if (mysql_conn!= NULL) {
        mysql_close(mysql_conn);
    }
}

MySQLManager& MySQLManager::getInstance() {
    static MySQLManager instance;
    return instance;
}

MYSQL* MySQLManager::getConnection() {
    return mysql_conn;
}

void MySQLManager::closeConnection() {
    if (mysql_conn!= NULL) {
        mysql_close(mysql_conn);
        mysql_conn = NULL;
    }
}

std::string MySQLManager::escapeString(const std::string& str) {
    if (mysql_conn == NULL) return str;  // 如果连接未建立，返回原字符串（会有警告）
    
    // 优化：使用栈数组代替堆分配，避免new/delete开销
    // MySQL要求转义后的字符串最多是原长度的2倍+1
    constexpr size_t MAX_STACK_SIZE = 512;  // 栈数组大小
    char stack_buf[MAX_STACK_SIZE];
    char* escaped_buf = nullptr;
    size_t buf_size = str.length() * 2 + 1;
    
    // 如果字符串较小，使用栈数组；否则使用堆分配
    if (buf_size <= MAX_STACK_SIZE) {
        escaped_buf = stack_buf;
    } else {
        escaped_buf = new char[buf_size];
    }
    
    unsigned long len = mysql_real_escape_string(mysql_conn, escaped_buf, str.c_str(), str.length());
    std::string result(escaped_buf, len);
    
    // 只有堆分配才需要释放
    if (escaped_buf != stack_buf) {
        delete[] escaped_buf;
    }
    
    return result;
}

bool MySQLManager::enqueueSql(const std::string& sql) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.size() >= maxQueueSize) {
        // 丢弃最新任务以保护 IO 线程不被阻塞
        return false;
    }
    queue.push_back(sql);
    cv.notify_all();  // 通知所有 worker 线程（因为可能有多个 worker 在等待）
    return true;
}

void MySQLManager::workerLoop(int workerId) {
    MYSQL* my_conn = worker_conns[workerId];
    if (my_conn == NULL) return;
    
    while (true) {
        std::string sql;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return stopping || !queue.empty(); });
            if (stopping && queue.empty()) break;
            if (queue.empty()) continue;  // 避免虚假唤醒
            sql = queue.front();
            queue.pop_front();
        }
        // 每个 worker 独立执行 SQL，真正的并行
        if (mysql_query(my_conn, sql.c_str())) {
            std::cerr << "mysql_query() failed in worker " << workerId << ": " << mysql_error(my_conn) << std::endl;
        }
    }
}