CXX = g++
CXXFLAGS = -Wall -Wextra -Weffc++ -Wold-style-cast -Woverloaded-virtual -std=c++17 -pedantic -Werror -g
LIBS = -pthread -lbcm2835

raspGPIO: main.o raspberry.o
	$(CXX) $(CXXFLAGS) $(LIBS) main.o raspberry.o -o raspGPIO

Raspberry.o: raspberry.cpp raspberry.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) raspberry.cpp -o raspberry.o

main.o: main.cpp raspberry.h
	$(CXX) -c $(CXXFLAGS) $(LIBS) main.cpp -o main.o

clean:
	rm main.o Raspberry.o raspGPIO

.PHONY: clean
