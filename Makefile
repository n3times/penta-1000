all:
	make -C cli
	make -C minimalistic
	make -C tests

lib:
	make lib -C cli
	make lib -C minimalistic

clean:
	make clean -C cli
	make clean -C minimalistic
	make clean -C tests
