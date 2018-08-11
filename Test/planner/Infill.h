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
#include "EepromReader.h"
#include "EepromWriter.h"

class Infill {

public:
	enum Type {
		ConstantFill, DrainFloodFill
	};

	Infill() {
	}

	virtual resource_t check(time_t mTime) = 0;

	virtual void dump(Print &aPrint) = 0;
	virtual void save(EepromWriter &aWriter) = 0;

	virtual ~Infill() {
	}


};

/**
 * Always return the same resource.
 */
class ConstantFill: public Infill {
	const resource_t mResource;

public:
	ConstantFill(resource_t aRes) :
			mResource(aRes) {
	}

	ConstantFill(const uint8_t aVersion, EepromReader &aReader) {
		aReader.read(mResource);
	}

	virtual void save(EepromWriter &aWriter) {
		aWriter.writeU8(Type::ConstantFill);
		aWriter.write(mResource);
	}

	virtual void dump(Print &aPrint) {
		aPrint.print("ConstantFill: ");
		aPrint.println(mResource);
	}

	resource_t check(time_t mTime) {
		return mResource;
	}
};

/**
 * Periodic return two resources Drain and Flood.
 */
class DrainFloodFill: public Infill {
	time_t mShiftSec;
	time_t mFloodSec;
	time_t mDrainSec;
	resource_t mFloodRes;
	resource_t mDrainRes;

public:
	DrainFloodFill(time_t aShiftSec, resource_t aFloodRes, time_t aFloodSec,
			resource_t aDrainRes, time_t aDrainSec) :
			mShiftSec(aShiftSec), mFloodRes(aFloodRes), mFloodSec(aFloodSec), mDrainRes(
					aDrainRes), mDrainSec(aDrainSec) {
	}

	DrainFloodFill(const uint8_t aVersion, EepromReader & aReader);

	virtual resource_t check(time_t mTime);

	virtual void dump(Print &aPrint);

	virtual void save(EepromWriter &aWriter);
};

#endif /* PLANNER_INFILL_H_ */
