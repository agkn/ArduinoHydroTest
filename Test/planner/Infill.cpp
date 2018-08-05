/*
 * Infill.cpp
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#include "Infill.h"

bool DrainFloodFill::check(uint32_t mTime) {
	uint32_t inTime = ( mTime + mShiftSec ) % (mDrainSec + mFloodSec);
	return inTime <=  mFloodSec;
}
