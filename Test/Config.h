/*
 * Config.h
 *
 *  Created on: 16 ���. 2018 �.
 *      Author: Andrey
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

#define CFG_SENSOR_THRE_LOW 		0 // 2 byte
#define CFG_SENSOR_THRE_HI 			2 // 2 byte
// level = level * LPF_A + value * ( 0xf - LPF_A ), must be 0x01 - 0x0e
#define CFG_SENSOR_LPF_A 			4 // 1 byte
// delay in second the pump off after a tank gets full
#define CFG_PUMP_OFF_DELAY      	5 // 1 byte
#define CFG_PUMP_SWITCH_INTERVAL	6 // 2 bytes


class Config {
public:
	static bool write(uint8_t aAddr, uint8_t aLen, uint8_t * aValues);
	static uint8_t * read(uint8_t aAddr, uint8_t aLen);
	static uint8_t read_u8(uint8_t aAddr);
	static uint16_t read_u16(uint8_t aAddr);
	static void dump();
	void init();
};

#endif /* CONFIG_H_ */
