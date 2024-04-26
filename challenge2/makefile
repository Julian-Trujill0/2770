CC = gcc
CFLAGS = -pthread -Wall -Wextra
SRCS_SERVER = HTTP_Server.c
SRCS_CLIENT = HTTP_Client.c
OBJS_SERVER = $(SRCS_SERVER:.c=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)
TARGETS = server client

all: $(TARGETS)

server: $(OBJS_SERVER)
        $(CC) $(CFLAGS) -o server $(OBJS_SERVER)

client: $(OBJS_CLIENT)
        $(CC) $(CFLAGS) -o client $(OBJS_CLIENT)

clean:
        rm -f $(OBJS_SERVER) $(OBJS_CLIENT) $(TARGETS)