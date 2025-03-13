CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pthread -lssl -lcrypto
OPENSSL_INC = -I/usr/include/openssl
OPENSSL_LIB = -L/usr/lib

# Target that ensures build-essential and libssl-dev are installed
install: 
	@echo "Checking for build-essential and libssl-dev..."
	@dpkg -s build-essential &>/dev/null || (echo "Installing build-essential..." && sudo apt update && sudo apt install -y build-essential)
	@dpkg -s libssl-dev &>/dev/null || (echo "Installing libssl-dev..." && sudo apt install -y libssl-dev)

all: install server client

server: src/server.cpp
	$(CC) $(CFLAGS) $(OPENSSL_INC) $(OPENSSL_LIB) src/server.cpp -o server

client: src/client.cpp
	$(CC) $(CFLAGS) $(OPENSSL_INC) $(OPENSSL_LIB) src/client.cpp -o client

clean:
	rm -f server client
