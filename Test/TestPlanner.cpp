#include "runcfg.h"
#if TEST == PLANNER
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include "planner/Planner.h"
#include "debug.h"

const resource_t FLOOD_RES = 1;
const resource_t DRAIN_RES = 2;
const resource_t LIGHT_RES = 3;

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
	Serial.begin(57600);

	time_t p1 = 8L * 3600 + 1 * 60 + 2; //  	0: 08:01:02
	time_t p2 = 20L * 3600 + 10 * 60 + 30; //	1: 20:10:30

	time_t p1_flood = 5;
	time_t p1_drain = 15;

	time_t p2_flood = 10;
	time_t p2_drain = 20;

	{

		Planner planner;

		planner.init(1);
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

		planner.save();
	}

	{
		Planner planner;

		dout(NULL);
		dout(NULL);
		dout("Restored dump");

		planner.init(false);
		planner.dump(Serial);

		assert(planner.getTimePin(0) == p1);
		assert(planner.getTimePin(1) == p2);
		assert(planner.getTimePin(2) == NOT_A_TIME);
		assert(planner.getTimePin(3) == NOT_A_TIME);
	}

	dout("All right");
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(LED_BUILTIN, HIGH);

	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}

#endif
