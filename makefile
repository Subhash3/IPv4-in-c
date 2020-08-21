CC = gcc
CFLAGS = -Wall

all: IPv4.o uInt32BitManipulations.o
		$(CC) $(CFLAGS) IPv4.o uInt32BitManipulations.o main.c -o main -lm

IPv4.o: IPv4.c
		$(CC) $(CFLAGS) -c IPv4.c
uInt32BitManipulations.o: uInt32BitManipulations.c
		$(CC) $(CFLAGS) -c uInt32BitManipulations.c
clean: 
		rm *.o