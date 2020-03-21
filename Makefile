all:
	make -C cli
	make -C mini
	make -C common/tests
	make -C python

clean:
	make clean -C cli
	make clean -C mini
	make clean -C common/tests
	make clean -C python
