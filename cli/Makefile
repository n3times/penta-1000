CC := gcc
SRC := ../engine/src
INC := ../engine/include

all: mini full

mini: lib
	$(CC) -Wall -I$(INC) -L. -lpenta1000 -lncurses mini.c -o mini

full: lib
	$(CC) -Wall -I$(INC) -L. -lpenta1000 full.c -o full

lib:
	$(CC) -c -Wall -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/game $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/game/*.c
	ar rcs libpenta1000.a *.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f a.out
