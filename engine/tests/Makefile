CC := gcc
SRC := ../src
INC := ../include

all: lib
	$(CC) -Wall -I$(INC) -L. -lpenta1000 *.c
	rm -f *.a

lib: obj
	ar rcs libpenta1000.a *.o
	rm -f *.o

obj:
	$(CC) -c -Wall -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/game $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/game/*.c

clean:
	rm -f *.o
	rm -f *.a
	rm -f a.out
