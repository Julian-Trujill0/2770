CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread -lssl -lcrypto

all: server client

server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client

clean:
	rm -f server client
