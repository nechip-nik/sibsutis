G++ = g++
CFLAGS = -Wall -Wextra -lboost_system -lpthread
TARGETS = server client

all: $(TARGETS)

server: server.cpp
		$(G++) $(CFLAGS) -o server server.cpp

client: client.cpp
		$(G++) $(CFLAGS) -o client client.cpp

clean: 
		rm -rf $(TARGETS)
.PHONY: all clean