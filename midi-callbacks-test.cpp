#include "midi-callbacks.h"
#include <iostream>

void printPitch(uint16_t p) {
	std::cout << "Pitch:\t" << static_cast<unsigned int>(p) << std::endl;
}

void printChannelPressure(uint8_t p) {
	std::cout << "CPressure:\t" << static_cast<unsigned int>(p) << std::endl;
}

void printPolyPressure(uint8_t a, uint8_t b) {
	std::cout << "PolyKeyPressure:\t" << static_cast<unsigned int>(a)
		<< static_cast<unsigned int>(b) << std::endl;
}

void printCC(uint8_t a, uint8_t b) {
	std::cout << "CC:\t" << static_cast<unsigned int>(a)
		<< "\t" << static_cast<unsigned int>(b) << std::endl;
}

void printNoteOn(uint8_t a, uint8_t b) {
	std::cout << "Note On:\t" << static_cast<unsigned int>(a)
		<< "\t" << static_cast<unsigned int>(b) << std::endl;
}

void printNoteOff(uint8_t a, uint8_t b) {
	std::cout << "Note Off:\t" << static_cast<unsigned int>(a)
		<< "\t" << static_cast<unsigned int>(b) << std::endl;
}

int main() {
	MidiStream ms {std::cin};

	ms.setCallback(Dyadic7Message::ControlChange, printCC);
	ms.setCallback(Monadic14Message::PitchBend, printPitch);
	ms.setCallback(Dyadic7Message::NoteOn, printNoteOn);
	ms.setCallback(Dyadic7Message::NoteOff, printNoteOff);
	ms.setCallback(Dyadic7Message::PolyKeyPressure, printPolyPressure);
	ms.setCallback(Monadic7Message::ChannelPressure, printChannelPressure);

	while (ms.hasNext()) {
		ms.next();
	}

	return EXIT_SUCCESS;
}
