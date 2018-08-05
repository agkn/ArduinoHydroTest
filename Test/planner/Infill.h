/*
 * Infill.h
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef PLANNER_INFILL_H_
#define PLANNER_INFILL_H_
#include <Arduino.h>
#include "types.h"

class Infill {
};

class DrainFloodFill: Infill {
	time_t mShiftSec;
	time_t mFloodSec;
	time_t mDrainSec;
public:
	DrainFloodFill(time_t aShiftSec, time_t aFloodSec, time_t aDrainSec):
		mShiftSec(aShiftSec), mFloodSec(aFloodSec), mDrainSec(aDrainSec) {};
	bool check(time_t mTime);
};

#endif /* PLANNER_INFILL_H_ */
