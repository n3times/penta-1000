all:
	make -C CLI
	make -C minimal_client
	make -C tests

lib:
	make lib -C CLI
	make lib -C minimal_client

clean:
	make clean -C CLI	
	make clean -C minimal_client
	make clean -C tests
