#ifndef MIDISTREAM
#define MIDISTREAM

#include <istream>
#include <map>
#include <cstdint>

#include "char-buffer.h"

enum class MidiMessageType {
	NoteOff,
	NoteOn,
	PolyKeyPressure,
	ControlChange,
	ProgramChange,
	ChannelPressure,
	PitchBend,
	Unknown,
};

typedef void (*CallbackFunction)(uint8_t);

class MidiStream {
public:
	MidiStream(std::istream &inputStream);
	bool hasNext();
	void next();

	void setCallback(MidiMessageType type, CallbackFunction callback);
private:
	std::istream &input;
	std::map<MidiMessageType, CallbackFunction> callbacks;

	void setCurrentCallback(MidiMessageType t);
	CallbackFunction currentCallback;

	CharBuffer buffer;
};

#endif
