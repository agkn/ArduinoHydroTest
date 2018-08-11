#include "runcfg.h"
#if TEST == SENSORS
#define __ASSERT_USE_STDERR

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

LevelController lc(LEVEL_PIN, LEVEL_VCC_PIN, PUMP_PIN);
RTClib clock;
DS3231 rtc;
Planner planner;
DHT dht(DHT_PIN, DHT_TYPE);

const uint8_t RES_PUMP = 1;

void cmd_run();
void dht_run();
void setup() {
	// init environment
	Serial.begin(9600);
	bool first = EepromSections::checkInited();
	Wire.begin();
	Config::init(first);
	Log::init(Serial);

	// init modules
	planner.init(first);
	lc.init();
	lc.enable(false);
	if (first) {
		planner.setTimePin(0, 8 * 3600 + 0 * 60 + 0);
		planner.setTimePin(1, 20 * 3600 + 0 * 60 + 0);
		planner.infillFlood(RES_PUMP, 0, 1, 5, 20);
		planner.infillFlood(RES_PUMP, 1, 0, 5, 30);
	}
}

void dumpDate() {
    DateTime now = clock.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);
    Serial.print(" temp:");
    Serial.print(rtc.getTemperature(), 2);
    Serial.println();

/*    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");*/
}

void loop() {
	bool pump = false;
	cmd_run(); // command processing
	dht_run(); // read sensors
	dumpDate(); // dump date
	DateTime now = clock.now();
	time_t time = now.hour() * 3600 + now.minute() * 60 + now.second();
	uint8_t idx = 0;

	lc.enable(pump);
	lc.run(); // level controller.
	delay(5000);
}

uint32_t cmd_readNumber() {
	uint32_t res;

	while(isDigit(Serial.peek())) {
		res = res * 10 + Serial.read() - 48;
	}

	return res;
}

void setTime(uint32_t aTime) {
	Serial.print("1e: ");
	Serial.println(aTime);

	DateTime dt(aTime);
	rtc.setYear(dt.year() - 2000);
	rtc.setMonth(dt.month());
	rtc.setDate(dt.day());
	rtc.setHour(dt.hour());
	rtc.setMinute(dt.minute());
	rtc.setSecond(dt.second());
}

void cmd_run() {
	if (!Serial.available()) {
		return;
	}
	const int cmd = Serial.read();
	Serial.print("Command: ");
	Serial.println((char)cmd);
	switch(cmd) {
	case 't' : setTime(cmd_readNumber()); break;
	case 'e' : lc.enable(true); break;
	case 'd' : lc.enable(false); break;
	}
}

void dht_run() {
	  // Reading temperature or humidity takes about 250 milliseconds!
	  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	  float h = dht.readHumidity();
	  // Read temperature as Celsius (the default)
	  float t = dht.readTemperature();

	  // Check if any reads failed and exit early (to try again).
	  if (isnan(h) || isnan(t)) {
	    Serial.println("Failed to read from DHT sensor!");
	    return;
	  }

	  Log::d(0).p(t).p(h).send();

	  Serial.print("Humidity: ");
	  Serial.print(h);
	  Serial.print(" %\t");
	  Serial.print("Temperature: ");
	  Serial.print(t, 2);
	  Serial.println(" *C ");
}

#endif // SENSORS
