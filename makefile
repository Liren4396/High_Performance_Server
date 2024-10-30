CC = g++
CFLAGS = -o

All: client server

client: client.cpp
	$(CC) $(CFLAGS) $@ $^
server: server.cpp
	$(CC) $(CFLAGS) $@ $^

.PHONY:clean
clean:
	rm -rf client server