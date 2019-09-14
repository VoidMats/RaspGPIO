CXX = g++
CXXFLAGS = -Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++17 -pedantic -Werror -g
LIBS = -pthread -lbcm2835 #-L/usr/local/lib/libbcm2835.a

rasp_client: main_client.o raspberryclient.o messagepacket.o
	$(CXX) $(CXXFLAGS) main_client.o raspberryclient.o messagepacket.o $(LIBS) -o rasp_client

messagepacket.o: messagepacket.cc messagepacket.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) messagepacket.cc -o messagepacket.o

raspberryclient.o: raspberryclient.cc raspberryclient.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberryclient.cc -o raspberryclient.o

main_client.o: main_client.cc raspberryclient.h messagepacket.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) main_client.cc -o main_client.o

clean:
	rm main_client.o .o raspberryserver.o connectclient.o rasp_client

.PHONY: clean