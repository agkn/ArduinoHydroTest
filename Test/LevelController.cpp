/*
 * LevelController.cpp
 *
 *  Created on: 16 июл. 2018 г.
 *      Author: Andrey
 */

#include "LevelController.h"
#include "Config.h"

#define FPA_FRACTIONAL 4
#define SENSOR_INIT_VAL 0xffff
// #define ALWAYS_SENSOR_VCC

LevelController::LevelController(uint8_t aSensorPin, uint8_t aSensorVccPin,
		uint8_t aRelayPin): _pumpChangeAllowed(0) {
	_sensorPin = aSensorPin;
	_sensorVccPin = aSensorVccPin;
	_pumpPin = aRelayPin;
}

void LevelController::init() {
	// sensor
	pinMode(_sensorPin, INPUT);
	pinMode(_sensorVccPin, OUTPUT);
	digitalWrite(_sensorVccPin, LOW);

	// pump
	pinMode(_pumpPin, OUTPUT);
	digitalWrite(_pumpPin, LOW);
	LevelController::configure();
}

void LevelController::configure() {
	_sensorLPF = Config::read_u8(CFG_SENSOR_LPF_A) & 0xf;
	_sensorThresholdHigh = Config::read_u16(CFG_SENSOR_THRE_HI) << FPA_FRACTIONAL;
	_sensorThresholdLow = Config::read_u16(CFG_SENSOR_THRE_LOW) << FPA_FRACTIONAL;
	_pumpOffDelay = Config::read_u8(CFG_PUMP_OFF_DELAY) * 1000; // convert to ms
	_pumpSwitchInterval = Config::read_u8(CFG_PUMP_SWITCH_INTERVAL);
	Serial.println("configure:");
	Serial.println(_sensorThresholdLow, HEX);
	Serial.println(_sensorThresholdHigh, HEX);
	Serial.println(_sensorLPF, HEX);
	Serial.println(_pumpOffDelay);
	Serial.println(_pumpSwitchInterval);
}

void LevelController::run() {
	LevelController::checkWaterLevel();
	LevelController::setPumpActive((!_full) && _enabled);
}

void LevelController::setPumpActive(bool aActive) {
	  if (aActive == _pumpActive) {
	    return;
	  }

	  const uint32_t now = millis();
	  Serial.print("Now: ");
	  Serial.print(now);
	  Serial.print(" Allowed: ");
	  Serial.println(_pumpChangeAllowed);

	  if ( (int) (now - _pumpChangeAllowed) > 0) {
	    digitalWrite(_pumpPin, aActive);
	    _pumpActive = aActive;
	    _pumpChangeAllowed = now + _pumpSwitchInterval;
	    Serial.print("pump: active: ");
	    Serial.println(aActive);
	  } else {
	    Serial.println("pump: wait allowed.");
	  }
}

void LevelController::setPumpModeDelayed(uint16_t aDelayMs) {
	  const unsigned long int now  = millis();
	  _pumpChangeAllowed = now + aDelayMs;

	  Serial.print("Set allowed: ");
	  Serial.print(now);
	  Serial.print(" -> ");
	  Serial.println(_pumpChangeAllowed);
}

void LevelController::checkWaterLevel() {

	if (!_enabled) {
		return;
	}

#ifndef ALWAYS_SENSOR_VCC
	digitalWrite(_sensorVccPin, HIGH); // * Подаем уровень логического 0 на вывод Vcc датчика
	delay(200);           // ждем готовности сенсора.
#endif
	uint32_t value = analogRead(_sensorPin);
#ifndef ALWAYS_SENSOR_VCC
	digitalWrite(_sensorVccPin, LOW); // * Подаем уровень логического 0 на вывод Vcc датчика
#endif

	if (_sensorValue == SENSOR_INIT_VAL ) {
		_sensorValue = value << FPA_FRACTIONAL;
	} else {
		_sensorLPF = 0xA;
		const uint32_t B = 0x10 - _sensorLPF;
		_sensorValue = _sensorValue * _sensorLPF + (value << FPA_FRACTIONAL) * B;
		_sensorValue >>= FPA_FRACTIONAL;
	}

	if (!_full && (_sensorValue < _sensorThresholdLow )) {
		_full = true;
		setPumpModeDelayed(_pumpOffDelay);
	} else if (_full && (_sensorValue > _sensorThresholdHigh)) {
		_full = false;
	}

	Serial.print(value);
	Serial.print(" - ");
	Serial.println(_sensorValue >> FPA_FRACTIONAL);
}

void LevelController::enable(bool aEnabled) {
	if (_enabled == aEnabled) {
		return;
	}

	_enabled = aEnabled;

	if (aEnabled) {
		_sensorValue = SENSOR_INIT_VAL;
#ifdef ALWAYS_SENSOR_VCC
		digitalWrite(_sensorVccPin, HIGH);
		delay(200);
#endif

	} else {

#ifdef ALWAYS_SENSOR_VCC
		digitalWrite(_sensorVccPin, LOW);
#endif
		_full = true;
	}
}
