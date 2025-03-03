// Util.h
#ifndef UTIL_H
#define UTIL_H

// C++标准库头文件
#include <iostream>
#include <vector>
// C 标准库头文件
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// 系统相关头文件
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>

void errif(bool condition, const char *errmsg);

#endif
