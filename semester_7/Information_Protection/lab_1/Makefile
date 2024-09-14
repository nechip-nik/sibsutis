CC = gcc
CFLAGS = -Wall

run:
	clang-format -i *.c *.h
	$(CC) $(CFLAGS) lib_cryp.c main.c -o test -lm

clean:
	rm -f test