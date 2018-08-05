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


const long DAY = 24 * 3600;

typedef unsigned int store_timepin_t;
const store_timepin_t TP_NOT_USED = 0xffff;


const int TIMEPINS_ADDR = EEPROM_PLANNER_ADDR;
const int TIMERANGE_ADDR = TIMEPINS_ADDR + sizeof(store_timepin_t) * TIMEPIN_NUMBER;
const int TIMERANGE_LAST = EEPROM_PLANNER_ADDR_END - sizeof(store_range_t);

bool TimeRange::check(time_t mTimeOfDay) {
	const time_t stop = (mStop < mStart) ? mStop + DAY : mStop;
	const time_t time = (mTimeOfDay < mStart) ? mTimeOfDay + DAY : mTimeOfDay;
	return mStart <= time && time < stop;
}

void Planner::init(bool aIsFirstRun) {
	if (aIsFirstRun) {
		// init points
		for(int i = 0; i < TIMEPIN_NUMBER; i++) {
			EEPROM.put(TIMEPINS_ADDR + i, TP_NOT_USED);
		}

		EEPROM.update(TIMERANGE_ADDR, InfillType::none);
	}

}

void Planner::read() {
	{
		store_timepin_t data;
		for(int i = 0; i < TIMEPIN_NUMBER; i++) {
			EEPROM.get(TIMEPINS_ADDR + i, data);
			mTimePins[i] = (TP_NOT_USED != data) ? data * 2 : NOT_A_TIME;
		}
	}
	{
		EepromReader reader(TIMERANGE_ADDR);
		store_range_t data;
		while(reader.mAddr <= TIMERANGE_LAST) {
			reader.read(data);

			Infill * infill = NULL;
			switch(data.type()) {
			case InfillType::pump: infill = (Infill *) new DrainFloodFill(
					(time_t) reader.readU16() * 2,
					(time_t) reader.readU16() * 2,
					(time_t) reader.readU16() * 2
					);
			}
		}
	}
}

static void TimeRange::add(TimeRange** aStart, TimeRange* aItem) {
	while(*aStart) {
		aStart = &(*aStart)->mNext;
	}
	*aStart = aItem;
	aItem->mNext = NULL;
}

bool Planner::setTimePin(uint8_t mIdx, time_t mTime) {
}

bool Planner::infillFlood(uint8_t aResId, uint8_t aStartIdx, uint8_t aStopIdx,
		uint8_t aFloodMin, uint8_t aDrainMin) {
}

uint8_t Planner::findActiveRes(time_t aTime, uint8_t& aIndex) {
}
