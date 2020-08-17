CC = gcc
CFLAGS = -Wall

all: IPv4.o
		$(CC) $(CFLAGS) IPv4.o main.c -o main -lm

IPv4.o: IPv4.c
		$(CC) $(CFLAGS) -c IPv4.c

clean: 
		rm *.o