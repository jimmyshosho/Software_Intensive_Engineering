CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread

all: client server

client: client.c csapp.c csapp.h 
server: server.c csapp.c csapp.h player.c deck.c

clean:
	rm -rf *~ client server *.o
