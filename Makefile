CC := gcc

all: lib
	$(CC) -Wall -I engine/api -I engine/include/ engine/src/*.c engine/src/comp/*.c cli/main.c
	rm engine/*.a

lib:
	make -C engine
