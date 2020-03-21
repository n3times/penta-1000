CC := gcc
SRC := ../common/src
INC := ../common/include

all:
	$(CC) -c -Wall -Werror -fpic -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/game $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/game/*.c
	gcc -shared -o libpenta1000.so *.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f a.out
	rm -rf __pycache__
