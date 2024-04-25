CC = gcc
CFLAGS = -Wall -pthread
SRCS = ThreadedMatrixMultiplication.c
OBJS = $(SRCS:.c=.o)
TARGET = ThreadedMatrixMultiplication
all: $(TARGET)
$(TARGET): $(OBJS)
 $(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
%.o: %.c
 $(CC) $(CFLAGS) -c $< -o $@
clean:
 rm -f $(OBJS) $(TARGET)