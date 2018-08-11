/*
 * Infill.cpp
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#include "Infill.h"
#include "../debug.h"

resource_t DrainFloodFill::check(time_t mTime) {
	time_t T = mDrainSec + mFloodSec;
	time_t inTime = ( mTime + mShiftSec ) % T;

//	dout("DrainFloodFill::check(%lu): inTime: %lu, mFloodSec: %lu, T: %lu ", mTime, inTime, mFloodSec, T);

	return inTime < mFloodSec ? mFloodRes : mDrainRes;
}

void DrainFloodFill::dump(Print &aPrint) {
	aPrint.println("DrainFloodFill: sft, fr, fd, dr, dd");
	aPrint.println(mShiftSec);
	aPrint.println(mFloodRes);
	aPrint.println(mFloodSec);
	aPrint.println(mDrainRes);
	aPrint.println(mDrainSec);
}

DrainFloodFill::DrainFloodFill(const uint8_t aVersion, EepromReader & aReader) {
	mShiftSec = aReader.readU16() * 2;
	aReader.read(mFloodRes);
	mFloodSec = aReader.readU16() * 2;
	aReader.read(mDrainRes);
	mDrainSec = aReader.readU16() * 2;
}

void DrainFloodFill::save(EepromWriter &aWriter) {
	aWriter.writeU8(Type::DrainFloodFill);

	aWriter.writeU16(mShiftSec >> 1);
	aWriter.write(mFloodRes);
	aWriter.writeU16(mFloodSec >> 1);
	aWriter.write(mDrainRes);
	aWriter.writeU16(mDrainSec >> 1);
}
