# Makefile for unit_test

CXX 	 := g++
CXXFLAGS := -Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++17 -pedantic -Werror -g
LIBS 	 := -pthread -lbcm2835 #-L/usr/local/lib/libbcm2835.a
INCLUDE  := -I../server

USER_ID := $(shell id -u)

test: test_server
	@echo "Recv directory: $(INSTALL_DIR)"
ifeq ($(USER_ID),0)
	@echo "You are running as root"
	@echo "Running test_server..."
	./test_server
else
	@echo "*** You need to run test_server as root ***"
endif

test_server: testserver.o ../server/raspberry.o ../server/raspberryserver.o ../server/connectclient.o
	@echo "Getting .o files from server..."
	$(CXX) $(CXXFLAGS) $(INCLUDE) testserver.o ../server/raspberry.o ../server/raspberryserver.o ../server/connectclient.o $(LIBS) -o test_server

TestServer.o: test_server.cc
	$(CXX) -c $(CXXFLAGS) $(LIBS) test_server.cc -o testserver.o

clean:
	rm -f testserver.o test_server

.PHONY: clean
