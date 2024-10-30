#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define PORT 8081
#define ADDRESS "127.0.0.1"

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}