#include "runcfg.h"
#ifndef TEST

#include <Arduino.h>
#include <DS3231.h>

#include "LevelController.h"
#include "EepromSections.h"
#include "DHT.h"
#include "Log.h"
#include "planner/Planner.h"
#include "Config.h"

/**
 * A7 - water sensor signal
 * 4  - BT Soft serial, RX, PinCode: 1243
 * 5  - BT Soft serial, TX
 * 6  - DHT22
 * 7  - Water sensor VCC
 * 8  - Pump activator.
 * A4 - RTC SDA
 * A5 - RTC SCL
 */
#define LEVEL_PIN A7
#define LEVEL_VCC_PIN 7
#define PUMP_PIN 8
#define RTC_SDA A4
#define RTC_SCL A5
#define DHT_PIN	6
#define DHT_TYPE DHT22

LevelController floodController(LEVEL_PIN, LEVEL_VCC_PIN, PUMP_PIN);
RTClib clock;
DS3231 rtc;
Planner planner;
DHT dht(DHT_PIN, DHT_TYPE);

enum Resources {
	NONE = 0,
	FLOOD,
	DRAIN,
	LIGHT
};

void setup() {
	// init environment
	Serial.begin(57600);
	Wire.begin();
	bool first = EepromSections::checkInited();
	dout("First time %d", first);

	Config::init(first);
	Log::init(Serial);

	// init modules
	planner.init(first);
	floodController.init();
	floodController.enable(false);
}

void loop() {
	bool flood = false;
	DateTime now = clock.now();
	dout("Now: %d-%02d-%02d %02d:%02d:%02d",
			now.year(), now.month(), now.day(),
			now.hour(), now.minute(), now.second());
	time_t time = now.hour() * 3600 + now.minute() * 60 + now.second();

	ResourceIterator * iter = planner.getResourceIterator(time);
	while(resource_t res = iter->next()) {
		dout("Active resource %d", res);
		flood = flood || (Resources::FLOOD == res);
	}

	floodController.enable(flood);
	floodController.run(); // level controller.
	delay(5000);
}

#endif // TEST
