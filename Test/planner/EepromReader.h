/*
 * EepromReader.h
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef PLANNER_EEPROMREADER_H_
#define PLANNER_EEPROMREADER_H_
#include <Arduino.h>
#include <EEPROM.h>
#include "../debug.h"

class EepromReader {
public:
	int mAddr;
	EepromReader(int aAddr): mAddr(aAddr) {};

	template<typename T> T& read(T & aData);

	uint16_t readU16();
	uint8_t readU8();
};

template<typename T>
inline T& EepromReader::read(T& aData) {
	EEPROM.get(mAddr, aData);
//	dout("Read eeprom %d bytes at %d", sizeof(T), mAddr);
	mAddr += sizeof(T);
	return aData;
}

#endif /* PLANNER_EEPROMREADER_H_ */
