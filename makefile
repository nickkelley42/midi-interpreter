all: midi-callbacks-test

midi-callbacks-test: midi-callbacks-test.cpp
midi-callbacks-test: midi-callbacks.o
midi-callbacks-test: char-buffer.o
midi-callbacks-test midi-callbacks.o: midi-callbacks.h

midi-callbacks.o: midi-callbacks.cpp
midi-callbacks.o: char-buffer.o

char-buffer.o midi-callbacks.o: char-buffer.h

char-buffer.o: char-buffer.cpp
