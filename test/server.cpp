#include "Server.h"

#include "EventLoop.h"

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    
    server->OnConnect([](Connection* conn) {
        conn->Read();
    });
    loop->loop();
    delete server;
    delete loop;
    return 0;
}
