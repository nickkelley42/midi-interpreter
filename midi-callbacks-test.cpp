#include "midi-callbacks.h"
#include <iostream>

int main() {
	MidiStream ms {std::cin};

	while (ms.hasNext()) {
		ms.next();
	}

	return EXIT_SUCCESS;
}
