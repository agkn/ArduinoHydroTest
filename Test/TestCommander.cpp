#include "runcfg.h"
#if TEST == COMMANDER
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include "debug.h"
#include "command/Commander.h"
#include "test/Planner.h"
#include "test/RTC.h"

Mock::Planner planner;
Mock::RTC rtc;

Commander commander(&planner, &rtc);

void setup() {
	Serial.begin(57600);
	commander.init(&Serial);

	Serial.println("Ready!");
}

void loop() {
	commander.run();
	delay(1000);
}


#endif
