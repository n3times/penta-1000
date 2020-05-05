CC := gcc
SRC := ../app_engine/src
INC := ../app_engine/include

all: mini full

mini: lib
	$(CC) -Wall -I$(INC) -Lbin -lpenta1000 -lncurses mini.c -o bin/mini

full: lib
	$(CC) -Wall -I$(INC) -Lbin -lpenta1000 full.c -o bin/full

lib: obj
	mkdir -p bin
	ar rcs bin/libpenta1000.a *.o
	rm *.o

obj:
	$(CC) -c -Wall -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/hilo -I$(SRC)/hilo2 $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/hilo/*.c $(SRC)/hilo2/*.c

clean:
	rm -f *.o
	rm -f -r bin
