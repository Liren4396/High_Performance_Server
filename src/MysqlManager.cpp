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
}

MySQLManager::~MySQLManager() {
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