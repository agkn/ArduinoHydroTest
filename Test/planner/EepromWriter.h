/*
 * EepromWriter.h
 *
 *  Created on: 11 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef PLANNER_EEPROMWRITER_H_
#define PLANNER_EEPROMWRITER_H_
#include <Arduino.h>
#include <EEPROM.h>
#include "../debug.h"

class EepromWriter {
public:
	int mAddr;
	EepromWriter(int aAddr): mAddr(aAddr) {};

	template<typename T> T& write(T & aData) {
			EEPROM.put(mAddr, aData);
			dout("Write eeprom %d bytes at %d", sizeof(T), mAddr);
			mAddr += sizeof(T);
	}

	void writeU8(uint8_t aData) {
		write(aData);
	}

	void writeU16(uint16_t aData) {
		write(aData);
	}
};

#endif /* PLANNER_EEPROMWRITER_H_ */
