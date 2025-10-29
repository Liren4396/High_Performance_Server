cmake_minimum_required(VERSION 3.10)
project(HighPerformanceServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 尝试查找 MySQL 客户端库，如果找不到则设为可选
find_library(MYSQLCLIENT_LIB mysqlclient PATHS /usr/lib64/mysql /usr/lib /usr/lib/x86_64-linux-gnu)

include_directories(/usr/include/mysql)

set(SOURCES_SERVER
    server.cpp
    src/Config.cpp
    src/ThreadPool.cpp
    src/MysqlManager.cpp
    src/Manager.cpp
    src/Server.cpp
    src/Channel.cpp
    src/Buffer.cpp
    src/Epoll.cpp
    src/InetAddress.cpp
    src/Socket.cpp
    src/Util.cpp
    src/EventLoop.cpp
    src/Acceptor.cpp
    src/Connection.cpp
    src/BufferPool.cpp
)

set(SOURCES_CLIENT
    client.cpp
    src/Config.cpp
    src/ThreadPool.cpp
    src/MysqlManager.cpp
    src/Manager.cpp
    src/Server.cpp
    src/Channel.cpp
    src/Buffer.cpp
    src/Epoll.cpp
    src/InetAddress.cpp
    src/Socket.cpp
    src/Util.cpp
    src/EventLoop.cpp
    src/Acceptor.cpp
    src/Connection.cpp
    src/BufferPool.cpp
)

add_executable(client ${SOURCES_CLIENT})
if(MYSQLCLIENT_LIB)
    target_link_libraries(client pthread ${MYSQLCLIENT_LIB})
else()
    target_link_libraries(client pthread)
endif()

add_executable(server ${SOURCES_SERVER})
if(MYSQLCLIENT_LIB)
    target_link_libraries(server pthread ${MYSQLCLIENT_LIB})
else()
    target_link_libraries(server pthread)
endif()

add_custom_target(custom_clean
    COMMAND rm -rf client server
    COMMENT "Cleaning up the build directory"
)
