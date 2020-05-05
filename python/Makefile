CC := gcc
SRC := ../app_engine/src
INC := ../app_engine/include

SRC_FILES = $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/hilo/*.c $(SRC)/hilo2/*.c

all: lib

lib: obj
	$(CC) -shared -o libpenta1000.so *.o
	rm -f *.o

obj:
	$(CC) -c -Wall -Werror -fpic -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/hilo -I$(SRC)/hilo2 $(SRC_FILES)
	$(CC) -shared -o libpenta1000.so *.o

clean:
	rm -f *.o
	rm -f *.so
	rm -rf __pycache__
