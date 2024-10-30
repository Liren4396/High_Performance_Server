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

#define PORT 8081
#define ADDRESS "127.0.0.1"

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}