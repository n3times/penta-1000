all:
	make -C CLI
	make -C tests

lib:
	make lib -C CLI

min:
	make min -C CLI

clean:
	make clean -C CLI	
	make clean -C tests
