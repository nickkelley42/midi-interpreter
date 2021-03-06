CXXFLAGS += -std=c++17
CC = g++
CXX = $(CC)

all: midi-callbacks-test

midi-callbacks-test: midi-callbacks-test.o
midi-callbacks-test.o : midi-callbacks-test.cpp

midi-callbacks-test: midi-callbacks.o
midi-callbacks-test.o midi-callbacks.o: midi-callbacks.h
midi-callbacks.o: midi-callbacks.cpp

midi-callbacks-test midi-callbacks.o: char-buffer.o
char-buffer.o midi-callbacks.o: char-buffer.h
char-buffer.o: char-buffer.cpp

clean:
	- rm midi-callbacks-test
	- rm *.o
