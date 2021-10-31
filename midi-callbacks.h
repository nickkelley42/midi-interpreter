#ifndef MIDISTREAM
#define MIDISTREAM

#include <istream>
#include <map>
#include <cstdint>
#include <variant>

#include "char-buffer.h"

enum class CallbackType {
	Monadic7,
	Dyadic7,
	Monadic14
};

typedef void (*Callback7Monadic)(uint8_t);

enum class Monadic7Message {
	NoteOff,
	ProgramChange,
	ChannelPressure,
	Unknown
};
const std::map<uint8_t, Monadic7Message> Monadic7MessageInts {
	{ 0, Monadic7Message::NoteOff },
	{ 4, Monadic7Message::ProgramChange },
	{ 5, Monadic7Message::ChannelPressure },
};

typedef void (*Callback7Dyadic)(uint8_t, uint8_t);

enum class Dyadic7Message {
	NoteOn,
	ControlChange,
	PolyKeyPressure
};

const std::map<uint8_t, Dyadic7Message> Dyadic7MessageInts {
	{ 1, Dyadic7Message::NoteOn },
	{ 2, Dyadic7Message::PolyKeyPressure },
	{ 3, Dyadic7Message::ControlChange }
};

typedef void (*Callback14Monadic)(uint16_t);

enum class Monadic14Message {
	PitchBend
};

const std::map<uint8_t, Monadic14Message> Monadic14MessageInts {
	{ 6, Monadic14Message::PitchBend }
};

typedef std::variant<Monadic7Message, Dyadic7Message, Monadic14Message> Message;
typedef void (*voidCallback)();

class MidiStream {
public:
	MidiStream(std::istream &inputStream);
	bool hasNext();
	void next();

	void setCallback(Monadic7Message type, Callback7Monadic callback);
	void setCallback(Dyadic7Message type, Callback7Dyadic callback);
	void setCallback(Monadic14Message type, Callback14Monadic callback);
private:
	std::istream &input;

	std::map<Monadic7Message, Callback7Monadic> monad7_callbacks;
	std::map<Dyadic7Message, Callback7Dyadic> dyad7_callbacks;
	std::map<Monadic14Message, Callback14Monadic> monad14_callbacks;

	Message messageType;
	void setCurrentCallback(Message type);
	void (*currentCallback)(void);
	void execCallback();

	CharBuffer buffer;
};

#endif
