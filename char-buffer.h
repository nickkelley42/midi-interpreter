#ifndef CHARBUFFER
#define CHARBUFFER

#include <cstdint>

class CharBuffer {
public:
	CharBuffer();
	bool isPresent();
	uint8_t content();
	void set(uint8_t);
	void clear();

private:
	uint8_t buffer;
	bool present;
};

#endif
