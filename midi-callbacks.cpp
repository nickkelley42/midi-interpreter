#include "midi-callbacks.h"
#include <iostream>
#include <stdexcept>

MidiStream::MidiStream(std::istream &inputStream)
	: input(inputStream), currentCallback(NULL) {}

bool MidiStream::hasNext() {
	return input.good();
}

void MidiStream::setCallback(MidiMessageType type, CallbackFunction callback) {
	callbacks[type] = callback;
}

MidiMessageType typeFromStatus(uint8_t statusByte) {
	auto typeFlag = (statusByte & 0x70) >> 4;
	MidiMessageType type = MidiMessageType::Unknown;

	if (MidiMessageInts.count(typeFlag)) {
		type = MidiMessageInts.at(typeFlag);
	}

	return type;
}

void MidiStream::next() {
	unsigned char b;
	input.read((char*) &b, 1);

	if (b >= 0x80) {
		MidiMessageType type = typeFromStatus(b);
		currentCallback = callbacks.count(type) ? callbacks.at(type) : NULL;
	} else if (currentCallback != NULL) {
		currentCallback(b);
	}
}
