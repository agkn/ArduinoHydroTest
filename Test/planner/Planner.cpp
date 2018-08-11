/*
 * Planner.cpp
 *
 *  Created on: 27 июл. 2018 г.
 *      Author: Andrey
 */

#include "Planner.h"

#include <EEPROM.h>
#include "../EepromSections.h"
#include "EepromReader.h"
#include "Infill.h"
#include "../debug.h"
#include "Factory.h"

const long DAY = 24L * 3600;

typedef uint16_t store_timepin_t;
const store_timepin_t TP_NOT_USED = 0xffff;

const int TIMEPINS_ADDR = EEPROM_PLANNER_ADDR;
const int TIMERANGE_ADDR = TIMEPINS_ADDR
		+ sizeof(store_timepin_t) * TIMEPIN_NUMBER;

TimeRange::TimeRange(Planner *aPlanner, uint8_t aVersion, EepromReader & aReader) {
	mPlanner = aPlanner;
	this->mNext = NULL;
	this->mStart = aReader.readU16() * 2;
	this->mStop = aReader.readU16() * 2;
	this->mInfill = Factory::createInfill(aReader);
}

void TimeRange::save(EepromWriter & aWriter) {
	aWriter.writeU8(Type::TIME_RANGE);
	aWriter.writeU16(mStart >> 1);
	aWriter.writeU16(mStop >> 1);
	mInfill->save(aWriter);
}

time_t TimeRange::shift(time_t aTime){
		const time_t start = mPlanner->mTimePins[mStart];

		if (aTime < start) {
			aTime += DAY;
		}

		return aTime - start;
}

resource_t TimeRange::check(time_t aTimeOfDay) {
	time_t shift = this->shift(aTimeOfDay);

	if (shift >= length() ) {
		return false;
	}
	dout("Match");
	return NULL != mInfill ? mInfill->check(shift) : NOT_A_RES;
}

static void TimeRange::add(TimeRange** aStart, TimeRange* aItem) {
	while (*aStart) {
		aStart = &(*aStart)->mNext;
	}
	*aStart = aItem;
	aItem->mNext = NULL;
}

void TimeRange::dump(Print &aPrint) {
	aPrint.print("TimeRange s:");
	aPrint.print(timeToStr(start()));
	aPrint.print(" e:");
	aPrint.println(timeToStr(stop()));
	if (mInfill) {
		mInfill->dump(aPrint);
	}
}

void Planner::init(bool aIsFirstRun) {
	if (aIsFirstRun) {
		// init points
		for (int i = 0; i < TIMEPIN_NUMBER; i++) {
			EEPROM.put(TIMEPINS_ADDR + i, TP_NOT_USED);
		}

		EEPROM.update(TIMERANGE_ADDR, TimeRange::NONE);
	}

	read();
}

void Planner::read() {
	{
		store_timepin_t data;
		for (int i = 0; i < TIMEPIN_NUMBER; i++) {
			EEPROM.get(TIMEPINS_ADDR + i * sizeof(store_timepin_t), data);
			dout("Read TimePin: %d, %u", TIMEPINS_ADDR + i * sizeof(store_timepin_t), data);
			mTimePins[i] = (TP_NOT_USED != data) ? 2l * data : NOT_A_TIME;
		}
	}
	{
		EepromReader reader(TIMERANGE_ADDR);
		//dout("reader start: %d, %d", reader.mAddr, TIMERANGE_ADDR);
		while (reader.readU8() == TimeRange::Type::TIME_RANGE) {
			dout("Read TimeRange: %d", reader.mAddr);
			TimeRange::add(&mRange, Factory::createTimeRange(this, reader));
		}
	}
}

void Planner::save() {
	for(int i = 0; i < TIMEPIN_NUMBER; i++) {
		store_timepin_t data = (NOT_A_TIME == mTimePins[i]) ? TP_NOT_USED : (mTimePins[i] >> 1);
		dout("Save TimePin: %d, %u", TIMEPINS_ADDR + i * sizeof(store_timepin_t), data);
		EEPROM.put(TIMEPINS_ADDR + i * sizeof(store_timepin_t), data);
	}

	EepromWriter writer(TIMERANGE_ADDR);
//	dout("writer start: %d, %d", writer.mAddr, TIMERANGE_ADDR);
	for (TimeRange *range = mRange; range != NULL; range = range->mNext) {
		dout("Save TimeRange: %d", writer.mAddr);
		range->save(writer);
	}

	writer.writeU8(TimeRange::Type::NONE);
}

void Planner::dump(Print &aPrint) {
	aPrint.println("TimePins:");
	for (int i = 0; i < TIMEPIN_NUMBER; i++) {
		time_t s = mTimePins[i];

		aPrint.print(i);
		aPrint.print(": ");
		aPrint.print(s);
		aPrint.print(" ");
		aPrint.println(timeToStr(s));
	}

	aPrint.println("TimeRanges:");
	for (TimeRange *range = mRange; range != NULL; range = range->mNext) {
		range->dump(aPrint);
	}
}

ResourceIterator * Planner::getResourceIterator(time_t aTime) {
	return new ResourceIterator(this->mRange, aTime);
}

resource_t ResourceIterator::next() {
	while (mTimeRange) {
		// mTimeRange->dump(Serial);
		resource_t res = mTimeRange->check(mCheckTime);

		if (NOT_A_RES != res) {
			mRes = res;
			mTimeRange = mTimeRange->mNext;
			return res;

		} else {
			mTimeRange = mTimeRange->mNext;
		}
	}

	return NOT_A_RES;
}

void Planner::setTimePin(uint8_t mIdx, time_t mTime) {
	mTimePins[mIdx] = mTime;
}

bool Planner::infillFlood(
		uint8_t aStartIdx, uint8_t aStopIdx,
		resource_t aFloodRes, uint8_t aFloodMin,
		resource_t aDrainRes, uint8_t aDrainMin
		) {
	dout("infillFlood");
	TimeRange::add(&this->mRange,
			new TimeRange(this, new DrainFloodFill(
					0,
					aFloodRes, 60L * aFloodMin,
					aDrainRes, 60L * aDrainMin), aStartIdx, aStopIdx));
	return true;
}

bool Planner::infillConst(uint8_t aStartIdx, uint8_t aStopIdx, uint8_t aResId) {
	TimeRange::add(&this->mRange,
			new TimeRange(this, new ConstantFill(aResId), aStartIdx, aStopIdx));

	return true;
}

