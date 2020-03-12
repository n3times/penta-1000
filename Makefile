all:
	make -C CLI
	make -C tests

clean:
	make clean -C CLI	
	make clean -C tests
