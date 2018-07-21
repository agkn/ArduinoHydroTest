/*
 * Config.cpp
 *
 *  Created on: 16 ���. 2018 �.
 *      Author: Andrey
 */

#include <EEPROM.h>

#include "Config.h"

#define VAL_SENSOR_THRE_LOW 500
#define VAL_SENSOR_THRE_HI 550
#define VAL_SENSOR_LPF 0xA
#define VAL_PUMP_OFF_DELAY 10
#define VAL_PUMP_SWITCH_INTERVAL 1000
#define TWO_BYTE_VAL(VAL) lowByte(VAL), highByte(VAL)

typedef uint16_t magic_t;
const magic_t EEPROM_MAGIC = 0x1240;
#define EEPROM_MAGIC_ADDR 0
#define EEPROM_DATA_ADDR (EEPROM_MAGIC_ADDR + sizeof(magic_t))

uint8_t data[] = {
		TWO_BYTE_VAL(VAL_SENSOR_THRE_LOW), // CFG_SENSOR_THRE_LOW
		TWO_BYTE_VAL(VAL_SENSOR_THRE_HI), // CFG_SENSOR_THRE_HI
		VAL_SENSOR_LPF, // CFG_SENSOR_LPF_A
		VAL_PUMP_OFF_DELAY,
		TWO_BYTE_VAL(VAL_PUMP_SWITCH_INTERVAL),
};

bool Config::write(uint8_t aAddr, uint8_t aLen, uint8_t * aValues) {
	if (aAddr < 0 || sizeof(data) - aAddr < aLen ) {
		return false;
	}
	// update data array.
	memcpy(data + aAddr, aValues, sizeof(uint8_t) * aLen);
	// update eeprom backend
	for(int i = 0; i < aLen; i++) {
		EEPROM.update(EEPROM_DATA_ADDR + aAddr + i, aValues[i]);
	}

	return true;
}

static uint8_t * Config::read(uint8_t aAddr, uint8_t aLen) {
	return data + aAddr;
}

static uint8_t Config::read_u8(uint8_t aAddr) {
	return data[aAddr];
}

static uint16_t Config::read_u16(uint8_t aAddr) {
	return *((uint16_t *)(data + aAddr));
}

static void Config::dump() {
	for(int i = 0; i < sizeof(data); i++) {
		Serial.print(i);
		Serial.print(":");
		Serial.println(data[i], HEX);
	}
}

static void Config::init() {
	magic_t magic;
	EEPROM.get(EEPROM_MAGIC_ADDR, magic);
	if (magic == EEPROM_MAGIC) {
		EEPROM.get(EEPROM_DATA_ADDR, data);
	} else {
		EEPROM.put(EEPROM_DATA_ADDR, data);
		EEPROM.put(EEPROM_MAGIC_ADDR, magic);
	}
}
