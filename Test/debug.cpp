
#include "debug.h"
#include <Arduino.h>


#ifdef NODEBUG

char * timeToStr(unsigned long aTime) {
	return "";
}

#else

const unsigned long lastTime = 24L * 3600;
const int debugStrBuffLen = 200;
char debugStrBuff[debugStrBuffLen];

const int timeStrBuffLen = 10;
char timeStrBuff[timeStrBuffLen];

void _debug_out(const char *__func, const char *__file, int __lineno,
		const char *__fmt, ...) {

	if (NULL == __fmt || '\0' == *__fmt) {
		Serial.println();
		return;
	}

	char * file = strrchr(__file, '/');
	if (!file) {
		file = __file;
	} else {
		file++; // skip '/'
	}

	Serial.print(file);
	Serial.print('[');
	Serial.print(__lineno);
	Serial.print("]: ");
	va_list ap;
	va_start(ap, __fmt);
	vsnprintf(debugStrBuff, debugStrBuffLen, __fmt, ap);
	Serial.println(debugStrBuff);
	va_end(ap);
}


char * timeToStr(unsigned long aTime) {
	if (aTime >= lastTime) {
		aTime = lastTime;
	}

	int h = aTime / 3600;
	aTime = aTime % 3600;
	int m = aTime / 60;
	aTime = aTime % 60;
	snprintf(timeStrBuff, timeStrBuffLen, "%02d:%02d:%02d", h, m, aTime);
	return timeStrBuff;
}

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno,
		const char *__sexp) {
	char * file = strrchr(__file, '/');
	if (!file) {
		file = __file;
	} else {
		file++; // skip '/'
	}

	// transmit diagnostic informations through serial link.
	Serial.print("Assert failed: ");
	Serial.println(__func);
	Serial.print(file);
	Serial.print("[");
	Serial.print(__lineno, DEC);
	Serial.print("]: ");
	Serial.println(__sexp);
	Serial.flush();
	// abort program execution.
	abort();
}

#endif NODEBUG
