CC = gcc
CFLAGS = -Wall -Wextra
TARGET = server client

all: $(TARGET)

server: server.c
		$(CC) $(CFLAGS) -o server server.c

client: client.c
		$(CC) $(CFLAGS) -o client client.c

clean:
		rm -rf $(TARGET)

.PHONY: all clean