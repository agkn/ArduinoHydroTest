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
#include "../planner/Planner.h"
#include "Log.h"
#include "../RTC.h"

/*
 * Commands:
 * r - get resources
 * t - get time pins
 * j - get jobs
 * t - show current time
 *
 * t:HH:MM:SS 				- set current time
 * p:id:HH:MM				- set time pin
 * j:id1:id2:p:r1:m1:r2:m2	- add job flood-drain
 * j:id1:id2:c:r			- add job constant
 * r:id1:id2:p				- remove flood-drain job
 * r:id1:id2:c:r			- remove constant job
 * */

class Commander {
	Stream * mStream;
	BaseCmd * mCommands;
	Planner * mPlanner;
	LogStream * mLog;
	Hydra::RTC * mRTC;

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
		RES_BAD_ARGUMENT,
	};

	LogStream* out(const char aCmd);

	Commander(Planner * aPlanner, Hydra::RTC * aRTC);

	void init(Stream * aStream);

	void run();

	void reset();

	LogStream log();

	Planner * getPlanner() {
		return mPlanner;
	}

	Hydra::RTC * getRtc() {
		return mRTC;
	}
};

#endif /* COMMANDER_H_ */
