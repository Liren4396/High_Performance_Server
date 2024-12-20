#include "src/include/EventLoop.h"
#include "src/include/Server.h"
#include "src/include/Manager.h"
int main() {
    EventLoop *eventLoop = new EventLoop();
    Server *svr = new Server(eventLoop);
    eventLoop->loop();
    delete svr;
    delete eventLoop;
    return 0;
}