#include "runcfg.h"
#if TEST == PLANNER
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include <assert.h>
#include "planner/Planner.h"
#include "debug.h"

const resource_t FLOOD_RES = 1;
const resource_t DRAIN_RES = 2;
const resource_t LIGHT_RES = 3;

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno,
		const char *__sexp) {
	// transmit diagnostic informations through serial link.
	Serial.print("Assert failed: ");
	Serial.println(__func);
	Serial.print(__file);
	Serial.print("[");
	Serial.print(__lineno, DEC);
	Serial.print("]: ");
	Serial.println(__sexp);
	Serial.flush();
	// abort program execution.
	abort();
}

void checkTime(Planner & planner, const char * aTitle, time_t aTime, uint8_t aExpectedResId) {
	ResourceIterator * iter = planner.getResourceIterator(aTime);
	dout(NULL); // \n
	dout("Check %s: res %d. time: %s", aTitle, aExpectedResId,
			timeToStr(aTime));
	bool resourseAsExpected = false;
	while (iter->next()) {
		dout("Active resource: %d <> %d", iter->resource(), aExpectedResId);
		resourseAsExpected = resourseAsExpected
				|| (iter->resource() == aExpectedResId);
	}

	if (aExpectedResId) {
		assert(resourseAsExpected != 0);
	} else {
		assert(resourseAsExpected == 0);
	}
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(9600);
	{

		Planner planner;

		planner.init(1);

		time_t p1 = 8L * 3600 + 1 * 60 + 2; //  	0: 08:01:02
		time_t p2 = 20L * 3600 + 10 * 60 + 30; //	1: 20:10:30

		time_t p1_flood = 5;
		time_t p1_drain = 15;

		time_t p2_flood = 10;
		time_t p2_drain = 20;

		planner.setTimePin(0, p1);
		planner.setTimePin(1, p2);
		planner.infillFlood(0, 1, FLOOD_RES, p1_flood, DRAIN_RES, p1_drain);
		planner.infillFlood(1, 0, FLOOD_RES, p2_flood, DRAIN_RES, p2_drain);
		planner.infillConst(1, 0, LIGHT_RES); // 20:10:3 - 08:01:02

		planner.dump(Serial);
		Serial.println("Setup end");
		Serial.println();

		time_t time = 1L * 3600;
		// left border must match the starting region
		checkTime(planner, "Left border", p1, FLOOD_RES);

		// right border must match next region
		checkTime(planner, "Right border", p2, FLOOD_RES);

		// right border must match next region
		checkTime(planner, "Midnight", 0, 3);

		// right
		checkTime(planner, "Flood right", p1 + p1_flood * 60, DRAIN_RES);

		dout(NULL);
		dout("All right");

		planner.save();
	}

	{
		Planner planner;

		dout(NULL);
		dout(NULL);
		dout("Restored dump");

		planner.init(false);
		planner.dump(Serial);
	}
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(LED_BUILTIN, HIGH);

	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}

#endif
