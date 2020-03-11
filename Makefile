CC := gcc

all: lib
	$(CC) -Wall -Iengine/include -Lengine -lpenta1000 cli/main.c
	rm engine/*.a

lib:
	make -C engine

clean: a.out
	make clean -C engine
	rm a.out
