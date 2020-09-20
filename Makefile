CC=cc
CFLAGS=-g -Wall -Wextra 

all:
	$(CC) $(CFLAGS) client/main.c -o red
	$(CC) $(CFLAGS) server/main.c -o redd

