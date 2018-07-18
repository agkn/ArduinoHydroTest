/*
 * LevelController.h
 *
 *  Created on: 16 èþë. 2018 ã.
 *      Author: Andrey
 */
#ifndef LEVELCONTROLLER_H_
#define LEVELCONTROLLER_H_

#include <Arduino.h>

class LevelController {
	uint8_t _sensorPin;
	uint8_t _sensorVccPin;
	uint8_t _pumpPin;

	uint32_t _sensorValue = 0;
	uint32_t _sensorThresholdLow;
	uint32_t _sensorThresholdHigh;
	uint32_t _sensorLPF;

	bool _full;
	bool _enabled;
	uint16_t _pumpOffDelay;
	uint32_t _pumpChangeAllowed;
	uint32_t _pumpSwitchInterval;
	bool _pumpActive;

public:
	LevelController(uint8_t aSensorPin, uint8_t aSensorVccPin, uint8_t aRelayPin);
	void init();
	void configure();
	void run();
	void enable(bool aEnabled);

private:
	void checkWaterLevel();
	void setPumpModeDelayed(uint16_t aDelayMs);
	void setPumpActive(bool aActivate);
};

#endif /* LEVELCONTROLLER_H_ */
