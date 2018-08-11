/*
 * Planner.h
 *
 *  Created on: 27 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef PLANNER_PLANNER_H_
#define PLANNER_PLANNER_H_
#include <Arduino.h>
#include "types.h"
#include "Infill.h"

const int TIMEPIN_NUMBER = 4;

class TimeRange;
class ResourceIterator;

class Planner {
	friend TimeRange;

	time_t mTimePins[TIMEPIN_NUMBER];
	TimeRange *mRange;
	void read();

public:

	Planner(): mRange(NULL) {}

	/**
	 * Init planner from eeprom memory.
	 *
	 * if aIsFirstRun true then no data has been saved in the memory.
	 */
	void init(bool aIsFirstRun);

	void setTimePin(uint8_t mIdx, time_t mTime);

	time_t getTimePin(uint8_t aIdx) {
		return mTimePins[aIdx];
	}

	bool infillConst(uint8_t aStartIdx, uint8_t aStopIdx, uint8_t aResId);

	bool infillFlood(uint8_t aStartIdx, uint8_t aStopIdx,
			resource_t aFloodRes, uint8_t aFloodMin,
			resource_t aDrainRes, uint8_t aDrainMin);
	/**
	 * Returns resource id of an active fill. Search start from aIndex, aIndex will be set to next position after the call completes.
	 */
	ResourceIterator * getResourceIterator(time_t aTime);

	void dump(Print &aPrint);

	void save();

	void run();
};


class TimeRange {
	uint8_t mStart, mStop;
	Planner *mPlanner;
	Infill * mInfill;
	TimeRange * mNext;
	friend class ResourceIterator;
	friend class Planner;
public:

	enum Type {NONE, TIME_RANGE};

	TimeRange(Planner *aPlanner, Infill *aInfill, uint8_t aStart, uint8_t aStop) :
			mPlanner(aPlanner),
			mInfill(aInfill),
			mStart(aStart),
			mStop(aStop),
			mNext(NULL) {
	}

	TimeRange(Planner *aPlanner, uint8_t aVersion, EepromReader & aReader);

	void save(EepromWriter & aWriter);

	resource_t check(time_t aTimeOfDay);

	time_t start() {
		return mPlanner->mTimePins[mStart];
	}

	time_t stop() {
		return mPlanner->mTimePins[mStop];
	}

	/**
	 * return time spent after start of the range.
	 */
	time_t shift(time_t aTime);

	time_t length() {
		return this->shift(mPlanner->mTimePins[mStop]);
	}

	static void add(TimeRange ** aStart, TimeRange * aItem);

	void dump(Print &aPrint);
};

class ResourceIterator {
	TimeRange * mTimeRange;
	const time_t mCheckTime;
	resource_t mRes = NOT_A_RES;
public:
	ResourceIterator(TimeRange * aTimeRange, time_t aCheckTime): mTimeRange(aTimeRange), mCheckTime(aCheckTime) {}
	bool next();

	resource_t resource() {
		return mRes;
	}
};

#endif /* PLANNER_PLANNER_H_ */
