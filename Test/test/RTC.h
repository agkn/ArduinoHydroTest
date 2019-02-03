/*
 * RTC.h
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef TEST_RTC_H_
#define TEST_RTC_H_
#include "../RTC.h"

namespace Mock {

class RTC: public Hydra::RTC {
public:

	virtual bool setTime(uint8_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMin, uint8_t aSeconds) {
		return true;
	}

	virtual Hydra::DateTime now() {
		return Hydra::DateTime(18, 8, 23, 22, 47,13);
	}

	virtual ~RTC() {};
};

} /* namespace Mock */

#endif /* TEST_RTC_H_ */
