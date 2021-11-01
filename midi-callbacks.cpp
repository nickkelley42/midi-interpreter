#include "midi-callbacks.h"
#include <iostream>
#include <stdexcept>

MidiStream::MidiStream(std::istream &inputStream)
	: input(inputStream), currentCallback(NULL) {}

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

void MidiStream::next() {
	unsigned char b;
	input.read((char*) &b, 1);

	if (b >= 0x80) {
		Message type = typeFromStatus(b);
		setCurrentCallback(type);
	} else if (currentCallback != NULL) {
		input.unget();
		execCallback();
	}
}

void MidiStream::setCurrentCallback(Message type) {
	currentCallback = NULL;

	if (std::holds_alternative<Monadic7Message>(type)) {
		auto t = std::get<Monadic7Message>(type);
		if (monad7_callbacks.count(t)) {
			currentCallback = (voidCallback) (monad7_callbacks.at(t));
		}
	} else if (std::holds_alternative<Dyadic7Message>(type)) {
		auto t = std::get<Dyadic7Message>(type);
		if (dyad7_callbacks.count(t)) {
			currentCallback = (voidCallback) dyad7_callbacks.at(t);
		}
	} else if (std::holds_alternative<Monadic14Message>(type)) {
		auto t = std::get<Monadic14Message>(type);
		if (monad14_callbacks.count(t)) {
			currentCallback = (voidCallback) monad14_callbacks.at(t);
		}
	}
	messageType = type;
}

void MidiStream::execCallback() {
	uint8_t byte;
	input.read((char*) &byte, 1);

	if (currentCallback == NULL) { return; }

	if (std::holds_alternative<Monadic7Message>(messageType)) {
		((Callback7Monadic) currentCallback)(byte);
	} else if (std::holds_alternative<Dyadic7Message>(messageType)) {
		if (buffer.isPresent()) {
			uint8_t first = buffer.content();
			((Callback7Dyadic) currentCallback)(first, byte);
			buffer.clear();
		} else {
			buffer.set(byte);
		}
	} else if (std::holds_alternative<Monadic14Message>(messageType)) {
		throw "have not implemented this yet...";
	}
}
