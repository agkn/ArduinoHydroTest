/*
 * Log.cpp
 *
 *  Created on: 17 июл. 2018 г.
 *      Author: Andrey
 */

#include "Log.h"


#define SEND_TYPE(tType) LogStream& LogStream::p(tType aVal) { 	if (ready()) { next(); _stream->print(aVal); } return str; }

Print *_stream;
LogStream str;

LogStream::LogStream() {
}

LogStream& LogStream::start(char aType, uint8_t aMsgCode) {
	if (!_sent){
		LogStream::send();
	}
	if (ready()) {
		_stream->print(aType);
		next();
		_stream->print(aMsgCode);
	}
	return str;
}

void LogStream::send() {
	if (ready()) {
	_stream->println();
	_stream->flush();
	_sent = true;
	}
}
bool LogStream::ready() {
	return NULL != _stream  && _stream->availableForWrite();
}

void LogStream::next() {
	_stream->print(":");
}

SEND_TYPE(char)
SEND_TYPE(float)
SEND_TYPE(uint16_t)

static void Log::init(Print &aStream) {
	_stream = &aStream;
}
static LogStream Log::msg(char aType, uint8_t aMsgCode) {
	return str.start(aType, aMsgCode);
}
