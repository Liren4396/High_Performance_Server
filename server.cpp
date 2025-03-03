#include <iostream>

#include "src/include/ThreadPool.h"
#include "src/include/EventLoop.h"
#include "src/include/Server.h"
#include "src/include/Manager.h"
#include "src/include/Config.h"

/*
int highest_qps;
void startQPSMonitor() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int count = g_qps_counter.exchange(0);
        std::cout << "QPS: " << count << std::endl;
        highest_qps = highest_qps > count ? highest_qps : count;
        std::cout << "highest-QPS: " << highest_qps << std::endl;
    }
}*/

int main() {
    //ThreadPool pool(2);
    //pool.add(startQPSMonitor);

    EventLoop *eventLoop = new EventLoop();
    Server *svr = new Server(eventLoop);
    eventLoop->loop();
    
    delete svr;
    delete eventLoop;

    return 0;
}
