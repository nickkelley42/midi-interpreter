#include "midi-callbacks.h"
#include <iostream>

void printPitch(uint16_t p) {
	std::cout << "Pitch:\t" << static_cast<unsigned int>(p) << std::endl;
}

void printCC(uint8_t a, uint8_t b) {
	std::cout << "CC:\t" << static_cast<unsigned int>(a)
		<< "\t" << static_cast<unsigned int>(b) << std::endl;
}

int main() {
	MidiStream ms {std::cin};

	ms.setCallback(Dyadic7Message::ControlChange, printCC);
	ms.setCallback(Monadic14Message::PitchBend, printPitch);

	while (ms.hasNext()) {
		ms.next();
	}

	return EXIT_SUCCESS;
}
