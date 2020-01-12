
# Export the direcory to all sub-makefiles
export INSTALL_DIR := $(shell pwd)

all:
	make all -C client
	make all -C server

install:
	make install -C client
	make install -C server

test:
	make all -C server
	make test -C unit_tests

clean:
	make clean -C client
	make clean -C server