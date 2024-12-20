CC = g++
CFLAGS = -std=c++17 -pthread -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -o 
All: client server

client: client.cpp src/Util.cpp
	$(CC) $(CFLAGS) $@ $^
server: server.cpp  src/ThreadPool.cpp src/MysqlManager.cpp src/Manager.cpp src/Server.cpp src/Channel.cpp src/Buffer.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Util.cpp src/EventLoop.cpp src/Acceptor.cpp src/Connection.cpp
	$(CC) $(CFLAGS) $@ $^ -pthread -g

.PHONY:clean
clean:
	rm -rf client server