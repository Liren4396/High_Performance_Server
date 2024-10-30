#ifndef UTIL_H

#define UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>
#include <vector>
using namespace std;

void errif(bool condition, const char *errmsg);
#endif