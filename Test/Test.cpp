#include <Arduino.h>
#include <DS3231.h>

#include "LevelController.h"
#include "Config.h"
#include "DHT.h"

/**
 * A7 - water sensor signal
 * 6 - DHT22
 * 7  - water sensor vcc
 * 8  - pump activator.
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
RTClib RTC;
DS3231 rtc;
DHT dht(DHT_PIN, DHT_TYPE);

void cmd_run();
void dht_run();
void setup() {
	Serial.begin(9600);
	Wire.begin();
	lc.init();
	lc.enable(false);
}

void dumpDate() {

    DateTime now = RTC.now();

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
    Serial.print(rtc.getTemperature(), DEC);
    Serial.println();

/*    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");*/
}

void loop() {
	cmd_run();
	dht_run();
	dumpDate();
	lc.run();
	delay(1000);
}

uint32_t cmd_readNumber() {
	uint32_t res;

	while(isDigit(Serial.peek())) {
		res = res * 10 + Serial.read() - 48;
	}

	return res;
}

void setTime(uint32_t aTime) {
	Serial.print("Set time: ");
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
	  // Read temperature as Fahrenheit (isFahrenheit = true)
	  float f = dht.readTemperature(true);

	  // Check if any reads failed and exit early (to try again).
	  if (isnan(h) || isnan(t) || isnan(f)) {
	    Serial.println("Failed to read from DHT sensor!");
	    return;
	  }

	  // Compute heat index in Fahrenheit (the default)
	  float hif = dht.computeHeatIndex(f, h);
	  // Compute heat index in Celsius (isFahreheit = false)
	  float hic = dht.computeHeatIndex(t, h, false);

	  Serial.print("Humidity: ");
	  Serial.print(h);
	  Serial.print(" %\t");
	  Serial.print("Temperature: ");
	  Serial.print(t);
	  Serial.print(" *C ");
	  Serial.print(f);
	  Serial.print(" *F\t");
	  Serial.print("Heat index: ");
	  Serial.print(hic);
	  Serial.print(" *C ");
	  Serial.print(hif);
	  Serial.println(" *F");
}
