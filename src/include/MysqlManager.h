// MysqlManager.h
#include <mysql/mysql.h>
#include <iostream>
#include <string>

class MySQLManager {
public:
    static MySQLManager& getInstance();
    MYSQL* getConnection();
    void closeConnection();

private:
    MySQLManager();
    ~MySQLManager();
    MySQLManager(const MySQLManager&) = delete;
    MySQLManager& operator=(const MySQLManager&) = delete;

    MYSQL* mysql_conn;  // 存储MySQL连接句柄
    std::string host;    // MySQL服务器主机地址
    std::string user;    // 登录用户名
    std::string password; // 登录密码
    std::string database; // 要使用的数据库名称
};
