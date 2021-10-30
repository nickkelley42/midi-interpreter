#include "midi-callbacks.h"
#include <iostream>

void printMessage(const char* type, uint8_t val) {
	std::cout << type << ":\t" << val << std::endl;
}

void printNoteOn(uint8_t v) { printMessage("Note on", v); }
void printUnknown(uint8_t v) { printMessage("Unknown:", v); }

int main() {
	MidiStream ms {std::cin};

	ms.setCallback(MidiMessageType::NoteOn, printNoteOn);
	ms.setCallback(MidiMessageType::Unknown, printUnknown);

	while (ms.hasNext()) {
		ms.next();
	}

	return EXIT_SUCCESS;
}
