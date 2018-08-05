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

const int TIMEPIN_NUMBER = 4;


enum InfillType {
	none = 0,
	pump = 1,
	relay = 2,
};

class TimeRange {
public:
	const time_t mStart, mStop;
	const uint8_t mResId;
	TimeRange * mNext;
	TimeRange(uint8_t aResId, time_t aStart, time_t aStop): mResId(aResId), mStart(aStart), mStop(aStop), mNext(NULL) {}
	bool check(time_t mTimeOfDay);
	static void add(TimeRange ** aStart, TimeRange * aItem);
};

class Planner {
	time_t mTimePins[TIMEPIN_NUMBER];
	TimeRange *mRange;
	void read();
public:
	void init(bool aIsFirstRun);
	bool setTimePin(uint8_t mIdx, time_t mTime);
	bool infillFlood(uint8_t aResId, uint8_t aStartIdx, uint8_t aStopIdx, uint8_t aFloodMin, uint8_t aDrainMin);
	/**
	 * Returns resource id of an active fill. Search start from aIndex, aIndex will be set to next position after the call completes.
	 */
	uint8_t findActiveRes(time_t aTime, uint8_t& aIndex);

	void run();
};


#endif /* PLANNER_PLANNER_H_ */
