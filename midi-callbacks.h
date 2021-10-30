#ifndef MIDISTREAM
#define MIDISTREAM

#include <istream>

class MidiStream {
public:
	MidiStream(std::istream &inputStream);
	bool hasNext();
	void next();
private:
	std::istream &input;
};

#endif
