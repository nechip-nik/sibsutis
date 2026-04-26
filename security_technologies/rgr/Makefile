CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -O2
CFLAGS = -Wall -O2
LIBS = -lgmp

OBJS = ecdh.o sha3.o salsa20.o

all: server client

server: server.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o server server.cpp $(OBJS) $(LIBS)

client: client.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o client client.cpp $(OBJS) $(LIBS)

ecdh.o: ecdh.cpp ecdh.h utils.h
	$(CXX) $(CXXFLAGS) -c ecdh.cpp

sha3.o: sha3.cpp sha3.h
	$(CXX) $(CXXFLAGS) -c sha3.cpp

salsa20.o: salsa20.c salsa20.h
	$(CC) $(CFLAGS) -c salsa20.c

clean:
	rm -f server client *.o received_file.txt

run-server: server
	./server

run-client: client
	./client