/*
 * Log.h
 *
 *  Created on: 17 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef LOG_H_
#define LOG_H_
#include <Arduino.h>
#include <Stream.h>

class LogStream {
	bool _sent = true;
	Stream *mStream;

public:
	LogStream(Stream *aStream): mStream(aStream) {};
	LogStream* start(char aType);
	LogStream* p(int aType);
	LogStream* p(uint8_t aType);
	LogStream* p(char aVal);
	LogStream* p(float aVal);
	LogStream* p(uint16_t aVal);
	LogStream* p(const char * aVal);
	void send();
private:
	void next();
	bool ready();
};

class Log {
	static LogStream * msg(char aType, uint8_t aMsgCode);
public:
	static void init(Stream &aStream);
	inline static LogStream* d(uint8_t aMsgCode) { return Log::msg('d', aMsgCode); }
	inline static LogStream* w(uint8_t aMsgCode) { return Log::msg('w', aMsgCode); }
	inline static LogStream* e(uint8_t aMsgCode) { return Log::msg('e', aMsgCode); }
};

#endif /* LOG_H_ */
