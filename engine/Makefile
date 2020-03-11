all:
	gcc -c -Wall -Iinclude -Isrc -Isrc/comp -Isrc/game src/*.c src/comp/*.c src/game/*.c
	ar rcs libpenta1000.a *.o
	rm *.o

clean:
	rm -f *.o
	rm -f *.a
