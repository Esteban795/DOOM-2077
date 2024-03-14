CFLAGS = -std=c99 -Wall -Wextra -pedantic
CC = clang
FLAGS = -lm

main: main.o bullet.o player.o
	$(CC) -o main player.o bullet.o main.o -lm


bullet.o: bullet.c bullet.h
	$(CC)  $(CFLAGS) -c bullet.c 

player.o: player.c player.h
	$(CC)  $(CFLAGS) -c player.c 

main.o: main.c 
	$(CC)  $(CFLAGS) -c main.c

clean:
	rm -f main  *.o 

run: main
	./main

