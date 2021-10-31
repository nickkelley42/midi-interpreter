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

const std::map<uint8_t, MidiMessageType> MidiMessageInts {
	{ 0, MidiMessageType::NoteOff },
	{ 1, MidiMessageType::NoteOn },
	{ 2, MidiMessageType::PolyKeyPressure },
	{ 3, MidiMessageType::ControlChange },
	{ 4, MidiMessageType::ProgramChange },
	{ 5, MidiMessageType::ChannelPressure },
	{ 6, MidiMessageType::PitchBend }
};

typedef void (*CallbackFunction)(uint8_t);

typedef void (*Callback7Monadic)(uint8_t);
typedef void (*Callback7Dyadic)(uint8_t, uint8_t);
typedef void (*Callback14Mondadic)(uint16_t);

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
