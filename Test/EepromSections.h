/*
 * EepromSections.h
 *
 *  Created on: 26 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef EEPROMSECTIONS_H_
#define EEPROMSECTIONS_H_
#include <Arduino.h>

typedef uint16_t magic_t;
#define EEPROM_MAGIC_ADDR 0
#define EEPROM_CONFIG_ADDR (EEPROM_MAGIC_ADDR + sizeof(magic_t))
#define EEPROM_PLANNER_ADDR (EEPROM_CONFIG_ADDR + 32 * 2)
// 4 points + 4 ranges.
#define EEPROM_PLANNER_ADDR_END (4 * 2 + 4 * 16)
class EepromSections {
public:

	/**
	 * Return true if eeprom memory need to be inited, false if already contains data.
	 */
	static bool checkInited();
};

#endif /* EEPROMSECTIONS_H_ */
