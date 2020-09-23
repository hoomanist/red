CC=cc
CFLAGS=-g -Wall -Wextra -Wmisleading-indentation -Wimplicit-fallthrough

all:
	$(CC) $(CFLAGS) -lreadline client/main.c -o red
	$(CC) $(CFLAGS) server/main.c -o redd
clean:
	rm redd red
