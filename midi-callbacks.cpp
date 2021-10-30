#include "midi-callbacks.h"

MidiStream::MidiStream(std::istream &inputStream) : input(inputStream) {}

bool MidiStream::hasNext() {
	return input.good();
}

void MidiStream::next() {
	char c;
	input.get(c);
}


