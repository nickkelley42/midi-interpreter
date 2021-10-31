#include "char-buffer.h"
#include <stdexcept>

CharBuffer::CharBuffer() : present(false) {}

bool CharBuffer::isPresent() { return present; }

uint8_t CharBuffer::content() {
	if (!present) {
		throw std::out_of_range {"Attempted to read from empty buffer"};
	}

	return buffer;
}
