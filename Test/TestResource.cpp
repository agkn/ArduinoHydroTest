#include "runcfg.h"
#if TEST == RESOURCE
#define __ASSERT_USE_STDERR

#include <Arduino.h>
#include "debug.h"
#include "Resources.h"

Hydra::Resources res;


void setup() {
	Serial.begin(57600);

	res.init();

	Serial.println("Ready!");
}

void loop() {
	commander.run();
	delay(1000);
}


#endif
