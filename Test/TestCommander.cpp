#include "runcfg.h"
#if TEST == COMMANDER
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include "planner/Planner.h"
#include "debug.h"
#include "command/Commander.h"

Commander commander;

void setup() {
	Serial.begin(57600);
	commander.init(Serial);

	Serial.println("Ready!");
}

void loop() {
	commander.run();
	delay(1000);
}


#endif
