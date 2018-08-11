/*
 * EepromReader.cpp
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#include "EepromReader.h"


uint16_t EepromReader::readU16() {
	uint16_t data;
	return read(data);
}

uint8_t EepromReader::readU8() {
	uint8_t data;
	return read(data);
}
