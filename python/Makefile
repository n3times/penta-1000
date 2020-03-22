CC := gcc
SRC := ../common/src
INC := ../common/include

SRC_FILES = $(SRC)/*.c $(SRC)/calc/*.c $(SRC)/game/*.c

all: lib

lib:
	$(CC) -c -Wall -Werror -fpic -I$(INC) -I$(SRC) -I$(SRC)/calc -I$(SRC)/game $(SRC_FILES)
	$(CC) -shared -o libpenta1000.so *.o

clean:
	rm -f *.o
	rm -f *.so
	rm -rf __pycache__
