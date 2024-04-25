CC = gcc
CFLAGS = -unistd
SRCS = HTTP_SERVER.c
OBJS = $(SRCS:.c=.o)
TARGET = challenge2

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)