CC := gcc
SRC := ../common/src
INC := ../common/include

all:
	$(CC) -c -Wall -Werror -fpic -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/game $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/game/*.c
	$(CC) -shared -o libpenta1000.so *.o

clean:
	rm -f *.o
	rm -f *.so
	rm -f a.out
