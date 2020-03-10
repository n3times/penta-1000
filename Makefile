all:
	gcc -Wall -I engine/api -I engine/include/ engine/src/*.c engine/src/comp/*.c cli/main.c

