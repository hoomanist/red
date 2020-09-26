CC=cc
CFLAGS=-g -Wall -Wextra -Wmisleading-indentation -Wimplicit-fallthrough

all:
	$(CC) $(CFLAGS) -lreadline client/client.c -o red
	$(CC) $(CFLAGS) server/server.c -o redd
clean:
	rm redd red
