all:
	make -C CLI
	make -C tests

min:
	make min -C CLI

clean:
	make clean -C CLI	
	make clean -C tests
