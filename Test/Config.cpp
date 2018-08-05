/*
 * Config.cpp
 *
 *  Created on: 16 ���. 2018 �.
 *      Author: Andrey
 */

#include <EEPROM.h>

#include "Config.h"
#include "EepromSections.h"

#define TWO_BYTE_VAL(VAL) lowByte(VAL), highByte(VAL)

#define VAL_SENSOR_THRE_LOW 500
#define VAL_SENSOR_THRE_HI 550
#define VAL_SENSOR_LPF 0xA
#define VAL_PUMP_OFF_DELAY 10
#define VAL_PUMP_SWITCH_INTERVAL 1000

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
		EEPROM.update(EEPROM_CONFIG_ADDR + aAddr + i, aValues[i]);
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

/**
 * Returns true if memory was inited by default values, false valid values already stored.
 */
static void Config::init(bool aUseDefaultValues) {
	if (aUseDefaultValues) {
		// copy default values to eeprom
		EEPROM.put(EEPROM_CONFIG_ADDR, data);
	} else {
		// restore values from eeprom
		EEPROM.get(EEPROM_CONFIG_ADDR, data);
	}
}
