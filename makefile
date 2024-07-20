CC = gcc
CFLAGS = -Wall -Wextra

all: poker

poker: poker.c
	$(CC) $(CFLAGS) -o poker poker.c

clean:
	rm -f poker
