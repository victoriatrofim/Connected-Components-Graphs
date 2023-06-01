CC = gcc
CFLAGS = -Wall -g
build: main

main: tema3.o
	$(CC) -o tema3 $^ 

main.o: tema3.c
	$(CC) $(CFLAGS)-c $^
run: build
	./tema3 1

clean:
	rm -f *.o tema3
