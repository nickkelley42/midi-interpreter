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
typedef void (*Callback7Dyadic)(uint8_t, uint8_t);
typedef void (*Callback14Monadic)(uint16_t);

typedef std::variant<std::monostate, Callback7Monadic, Callback7Dyadic, Callback14Monadic> Callback;

enum class Monadic7Message {
	ProgramChange,
	ChannelPressure,
	Unknown
};

enum class Dyadic7Message {
	NoteOff,
	NoteOn,
	ControlChange,
	PolyKeyPressure
};

enum class Monadic14Message {
	PitchBend
};

typedef std::variant<Monadic7Message, Dyadic7Message, Monadic14Message> Message;

const std::map<uint8_t, Monadic7Message> Monadic7MessageInts {
	{ 4, Monadic7Message::ProgramChange },
	{ 5, Monadic7Message::ChannelPressure },
};

const std::map<uint8_t, Dyadic7Message> Dyadic7MessageInts {
	{ 0, Dyadic7Message::NoteOff },
	{ 1, Dyadic7Message::NoteOn },
	{ 2, Dyadic7Message::PolyKeyPressure },
	{ 3, Dyadic7Message::ControlChange }
};

const std::map<uint8_t, Monadic14Message> Monadic14MessageInts {
	{ 6, Monadic14Message::PitchBend }
};

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
	Callback callback;

	void setCallback(Callback7Monadic);
	void setCallback(Callback7Dyadic);
	void setCallback(Callback14Monadic);

	void execCallback(uint8_t);

	void execCallback(std::monostate, uint8_t);
	void execCallback(Callback7Monadic, uint8_t);
	void execCallback(Callback7Dyadic, uint8_t);
	void execCallback(Callback14Monadic, uint8_t);

	CharBuffer buffer;
};

#endif
