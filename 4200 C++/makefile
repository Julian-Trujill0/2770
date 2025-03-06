# Compiler and Linker
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lssl -lcrypto

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source and Object Files
SOURCES = $(SRC_DIR)/server.cpp $(SRC_DIR)/client.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Targets
TARGETS = $(BIN_DIR)/server $(BIN_DIR)/client

# Default Target
all: check_openssl $(TARGETS)

# Check and Install OpenSSL if Necessary
check_openssl:
	@if ! pkg-config --exists openssl; then \
		echo "OpenSSL not found. Installing..."; \
		sudo apt-get update && sudo apt-get install -y libssl-dev; \
	else \
		echo "OpenSSL is already installed."; \
	fi

# Build Server
$(BIN_DIR)/server: $(OBJ_DIR)/server.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build Client
$(BIN_DIR)/client: $(OBJ_DIR)/client.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile Source Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean Up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean check_openssl

