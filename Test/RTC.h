/*
 * RTC.h
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef RTC_H_
#define RTC_H_
#include "types.h"

namespace Hydra {

struct DateTime {
	const uint8_t year;
	const uint8_t month;
	const uint8_t day;
	const uint8_t hour;
	const uint8_t minutes;
	const uint8_t seconds;

	DateTime(uint8_t aYear,  uint8_t aMonth, uint8_t aDay,
			uint8_t aHour, uint8_t aMinutes, uint8_t aSeconds):
				year(aYear), month(aMonth), day(aDay),
				hour(aHour), minutes(aMinutes), seconds(aSeconds) {};

	time_t daySeconds() const  {
		return 3600l * hour + minutes * 60 + seconds;
	}
} ;

class RTC {

public:
	virtual bool setTime(uint8_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMin, uint8_t aSeconds);
	virtual DateTime now();
	virtual ~RTC() {};
};

}

#endif /* RTC_H_ */
