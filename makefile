CC = g++
CFLAGS = -o

All: client server

client: client.cpp Util.cpp
	$(CC) $(CFLAGS) $@ $^
server: server.cpp Channel.cpp Epoll.cpp InetAddress.cpp Socket.cpp Util.cpp
	$(CC) $(CFLAGS) $@ $^

.PHONY:clean
clean:
	rm -rf client server