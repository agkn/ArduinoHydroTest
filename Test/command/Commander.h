/*
 * Commander.h
 *
 *  Created on: 13 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMANDER_H_
#define COMMANDER_H_
#include <Arduino.h>
#include "BaseCmd.h"
/*
 * Commands:
 * r - get resources
 * t - get time pins
 * j - get jobs
 *
 * t:id:HH:MM				- set time pin
 * j:id1:id2:p:r1:m1:r2:m2	- add job flood-drain
 * j:id1:id2:c:r			- add job constant
 * d:id1:id2:p				- delete flood-drain job
 * d:id1:id2:c:r			- delete constant job
 * */

class Commander {
	Stream * mStream;
	BaseCmd * mCommands;

	void fail(int aResCode);
	void ok();
	void execute(char aCmd, char **aArgv, int aArcC);
	void proccessCommand();

public:
	enum Codes {
		RES_OK,
		RES_TOO_LONG,
		RES_BAD_FORMAT,
		RES_BAD_COMMAND,
	};

	void out(const char * aMessage);

	Commander();
	void init(Stream &aStream);
	void run();
	void reset();
};

#endif /* COMMANDER_H_ */
