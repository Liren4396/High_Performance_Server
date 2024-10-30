#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}