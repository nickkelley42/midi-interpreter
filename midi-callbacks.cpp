#include "midi-callbacks.h"
#include <iostream>
#include <stdexcept>

MidiStream::MidiStream(std::istream &inputStream)
	: input(inputStream), callback() {}

bool MidiStream::hasNext() {
	return input.good();
}

void MidiStream::setCallback(Monadic7Message type, Callback7Monadic callback) {
	monad7_callbacks[type] = callback;
}

void MidiStream::setCallback(Dyadic7Message type, Callback7Dyadic callback) {
	dyad7_callbacks[type] = callback;
}

void MidiStream::setCallback(Monadic14Message type, Callback14Monadic callback) {
	monad14_callbacks[type] = callback;
}

Message typeFromStatus(uint8_t statusByte) {
	auto typeFlag = (statusByte & 0x70) >> 4;

	Message type = Monadic7Message::Unknown;

	if (Monadic7MessageInts.count(typeFlag)) {
		type = Monadic7MessageInts.at(typeFlag);
	} else if (Dyadic7MessageInts.count(typeFlag)) {
		type = Dyadic7MessageInts.at(typeFlag);
	} else if (Monadic14MessageInts.count(typeFlag)) {
		type = Monadic14MessageInts.at(typeFlag);
	}

	return type;
}

constexpr uint8_t STATUSBIT = 0x80;

bool isStatusByte(uint8_t b) {
	return b & STATUSBIT;
}

void MidiStream::next() {
	unsigned char b;
	input.read((char*) &b, 1);

	if (isStatusByte(b)) {
		Message type = typeFromStatus(b);
		setCurrentCallback(type);
	} else {
		input.unget();
		execCallback();
	}
}

void MidiStream::setCurrentCallback(Message type) {
	messageType = type;
	buffer.clear();

	// The three nominal cases look nearly identical... I wonder
	// if they can be combined without sacrificing type safety?
	// Maybe using std::visit?
	//   https://en.cppreference.com/w/cpp/utility/variant/visit

	if (const auto t = std::get_if<Monadic7Message>(&type)) {
		if (monad7_callbacks.count(*t)) {
			callback = monad7_callbacks.at(*t);
		}
	} else if (const auto t = std::get_if<Dyadic7Message>(&type)) {
		if (dyad7_callbacks.count(*t)) {
			callback = dyad7_callbacks.at(*t);
		}
	} else if (const auto t = std::get_if<Monadic14Message>(&type)) {
		if (monad14_callbacks.count(*t)) {
			callback = monad14_callbacks.at(*t);
		}
	} else {
		// Default to setting the callback to monostate.
		// If we get an invalid status byte, I'm assuming that
		// any subsequent data bytes should be ignored.
		callback = std::monostate{};
	}
}

uint8_t readByte(std::istream& input) {
	uint8_t byte;
	input.read((char*)(&byte), 1);
	return byte;
}

void MidiStream::execCallback() {
	auto f = [this](auto&& cb) {
		uint8_t b = readByte(input);
		execCallback(cb, b);
	};
	std::visit(f, callback);
}

void MidiStream::execCallback(Callback7Monadic cb, uint8_t b) {
	cb(b);
}

void MidiStream::execCallback(Callback7Dyadic cb, uint8_t b) {
	if (buffer.isPresent()) {
		uint8_t first = buffer.content();
		cb(first, b);
		buffer.clear();
	} else {
		buffer.set(b);
	}
}

void MidiStream::execCallback(Callback14Monadic cb, uint8_t b) {
	if (buffer.isPresent()) {
		uint8_t first = buffer.content();
		uint16_t value = (b << 7) + first;

		cb(value);
		buffer.clear();
	} else {
		buffer.set(b);
	}
}

void MidiStream::execCallback(std::monostate m, uint8_t b) {}
