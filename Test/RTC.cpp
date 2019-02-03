/*
 * RTC.cpp
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: Andrey
 */

#include "RTC.h"
#include <DS3231.h>

namespace Hydra {

bool RTC::setTime(uint8_t aYear, uint8_t aMonth, uint8_t aDay,
		uint8_t aHour, uint8_t aMin, uint8_t aSeconds) {
	DS3231 rtc;

	rtc.setYear(aYear);
	rtc.setMonth(aMonth);
	rtc.setDate(aDay);
	rtc.setHour(aHour);
	rtc.setMinute(aMin);
	rtc.setSecond(aSeconds);

	return true;
}

DateTime Hydra::RTC::now() {
	RTClib rtc;
	::DateTime now = rtc.now();

	return Hydra::DateTime(now.year() - 2000, now.month(), now.day(), now.hour(), now.minute(), now.second());
}

}
