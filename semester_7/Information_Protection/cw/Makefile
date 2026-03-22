CC = gcc
CFLAGS = -Wall

run:
	clang-format -i *.c 
	$(CC) $(CFLAGS) -o server server.c -lssl -lcrypto
	$(CC) $(CFLAGS) -o client client.c -lssl -lcrypto
clean:
	rm -f server
	rm -f client