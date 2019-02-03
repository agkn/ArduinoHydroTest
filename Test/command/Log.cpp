/*
 * Log.cpp
 *
 *  Created on: 17 июл. 2018 г.
 *      Author: Andrey
 */

#include "Log.h"
#include "../debug.h"

#define SEND_TYPE(tType) LogStream* LogStream::p(tType aVal) { 	if (ready()) { next(); mStream->print(aVal); } return this;}

LogStream *gStr;

LogStream* LogStream::start(char aType) {
	if (!_sent){
		LogStream::send();
	}
	if (ready()) {
		mStream->print(aType);
	}
	return gStr;
}

void LogStream::send() {
	if (ready()) {
		mStream->println();
		mStream->flush();
		_sent = true;
	}
}

bool LogStream::ready() {
	return NULL != mStream  && mStream->availableForWrite();
}

void LogStream::next() {
	mStream->print(":");
}

SEND_TYPE(char)
SEND_TYPE(float)
SEND_TYPE(int)
SEND_TYPE(uint8_t)
SEND_TYPE(uint16_t)
SEND_TYPE(const char *)

static void Log::init(Stream &aStream) {
	gStr = new LogStream(&aStream);
}

static LogStream * Log::msg(char aType, uint8_t aMsgCode) {
	if (!gStr) {
		dout("Log stream is not inited!");
		return new LogStream(NULL);
	}
	return gStr->start(aType)->p(aMsgCode);
}
