all:
	make -C cli
	make -C mini
	make -C common/tests

lib:
	make lib -C cli
	make lib -C mini

clean:
	make clean -C cli
	make clean -C mini
	make clean -C common/tests
