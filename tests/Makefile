CC := gcc
SRC := ../common/src
INC := ../common/include

all: lib
	$(CC) -Wall -I$(INC) -L. -lpenta1000 test_calc.c

lib:
	gcc -c -Wall -I$(INC) -I$(SRC) -I$(SRC)/comp -I$(SRC)/game $(SRC)/*.c $(SRC)/comp/*.c $(SRC)/game/*.c
	ar rcs libpenta1000.a *.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f a.out
