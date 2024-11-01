#include "src/EventLoop.h"
#include "src/Server.h"

int main() {
    EventLoop *eventLoop = new EventLoop();
    Server *svr = new Server(eventLoop);
    eventLoop->loop();
    delete svr;
    delete eventLoop;
    return 0;
}