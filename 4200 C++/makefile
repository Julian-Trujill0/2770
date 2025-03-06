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
all: check_dependencies $(TARGETS)

# Check and Install Dependencies if Necessary
check_dependencies:
	@which apt-get > /dev/null && { \
		echo "Updating package lists..."; \
		sudo apt-get update; \
		echo "Installing build-essential and libssl-dev..."; \
		sudo apt-get install -y build-essential libssl-dev; \
	} || { \
		echo "apt-get not found. Please install build-essential and libssl-dev manually."; \
	}

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

.PHONY: all clean check_dependencies
