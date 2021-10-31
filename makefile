all: midi-callbacks-test

CC = g++

midi-callbacks-test: midi-callbacks-test.o
midi-callbacks-test.o : midi-callbacks-test.cpp

midi-callbacks-test: midi-callbacks.o
midi-callbacks-test.o midi-callbacks.o: midi-callbacks.h
midi-callbacks.o: midi-callbacks.cpp

midi-callbacks-test midi-callbacks.o: char-buffer.o
char-buffer.o midi-callbacks.o: char-buffer.h
char-buffer.o: char-buffer.cpp
