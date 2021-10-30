
all: midi-callbacks-test

midi-callbacks-test: midi-callbacks-test.cpp
midi-callbacks-test: midi-callbacks.o
midi-callbacks-test: midi-callbacks.h
midi-callbacks.o: midi-callbacks.h midi-callbacks.cpp
