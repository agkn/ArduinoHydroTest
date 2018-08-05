/*
 * EepromSections.cpp
 *
 *  Created on: 26 июл. 2018 г.
 *      Author: Andrey
 */

#include "EepromSections.h"
#include <EEPROM.h>

const magic_t EEPROM_MAGIC = 0x1240;

static bool EepromSections::checkInited() {
	magic_t magic;
	EEPROM.get(EEPROM_MAGIC_ADDR, magic);
	if (magic == EEPROM_MAGIC) {
		return false;
	} else {
		EEPROM.put(EEPROM_MAGIC_ADDR, magic);
		return true;
	}
}
