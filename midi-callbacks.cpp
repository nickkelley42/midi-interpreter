#include "midi-callbacks.h"
#include <iostream>

MidiStream::MidiStream(std::istream &inputStream)
	: input(inputStream), currentCallback(NULL) {}

bool MidiStream::hasNext() {
	return input.good();
}

void MidiStream::setCallback(MidiMessageType type, CallbackFunction callback) {
	callbacks[type] = callback;
}

void MidiStream::next() {
	unsigned char b;
	input.read((char*) &b, 1);

	if (b >= 0x80) {
		auto type_flag = (b & 0x70) >> 4;
		MidiMessageType type;
		switch(type_flag) {
		case 0:
			type = MidiMessageType::NoteOff;
			break;
		case 1:
			type = MidiMessageType::NoteOn;
			break;
		case 2:
			type = MidiMessageType::PolyKeyPressure;
			break;
		case 3:
			type = MidiMessageType::ControlChange;
			break;
		case 4:
			type = MidiMessageType::ProgramChange;
			break;
		case 5:
			type = MidiMessageType::ChannelPressure;
			break;
		case 6:
			type = MidiMessageType::PitchBend;
			break;
		default:
			type = MidiMessageType::Unknown;
		}

		currentCallback = callbacks.count(type) ? callbacks.at(type) : NULL;
	} else if (currentCallback != NULL) {
		currentCallback(b);
	}
}
