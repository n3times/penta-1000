all:
	make -C cli
	make -C engine/tests
	make -C python

clean:
	make clean -C cli
	make clean -C engine/tests
	make clean -C python
