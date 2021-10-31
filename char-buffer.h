#ifndef CHARBUFFER
#define CHARBUFFER

#include <cstdint>

class CharBuffer {
public:
	CharBuffer();
	bool isPresent();
	uint8_t content();

private:
	uint8_t buffer;
	bool present;
};

#endif
