CXX = g++
CXXFLAGS = -Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++17 -pedantic -Werror -g
LIBS = -pthread -lbcm2835 #-L/usr/local/lib/libbcm2835.a

raspGPIO: main.o raspberry.o raspberryserver.o connectclient.o
	$(CXX) $(CXXFLAGS) main.o raspberry.o raspberryserver.o connectclient.o $(LIBS) -o raspGPIO

RaspberryServer.o: raspberryServer.cc raspberryserver.h 
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberryserver.cc -o raspberryserver.o

ConnectClient.o: connectclient.cc connectclient.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) connectclient.cc -o connectclient.o

Raspberry.o: raspberry.cpp raspberry.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberry.cpp -o raspberry.o

main.o: main.cpp raspberry.h raspberryserver.h connectclient.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) main.cpp -o main.o

clean:
	rm main.o Raspberry.o raspberryserver.o connectclient.o raspGPIO

.PHONY: clean
