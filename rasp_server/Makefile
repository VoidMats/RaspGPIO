CXX = g++
CXXFLAGS = -Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++17 -pedantic -Werror -g
LIBS = -pthread -lbcm2835 #-L/usr/local/lib/libbcm2835.a

rasp_server: main_server.o raspberry.o raspberryserver.o connectclient.o
	$(CXX) $(CXXFLAGS) main_server.o raspberry.o raspberryserver.o connectclient.o $(LIBS) -o rasp_server

RaspberryServer.o: raspberryServer.cc raspberryserver.h 
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberryserver.cc -o raspberryserver.o

ConnectClient.o: connectclient.cc connectclient.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) connectclient.cc -o connectclient.o

Raspberry.o: raspberry.cpp raspberry.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberry.cpp -o raspberry.o

main_server.o: main_server.cpp raspberry.h raspberryserver.h connectclient.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) main_server.cpp -o main_server.o

clean:
	rm main_server.o Raspberry.o raspberryserver.o connectclient.o rasp_server

.PHONY: clean
