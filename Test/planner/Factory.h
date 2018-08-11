/*
 * Factory.h
 *
 *  Created on: 11 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef PLANNER_FACTORY_H_
#define PLANNER_FACTORY_H_
#include "Infill.h"
#include "Planner.h"

const uint8_t FACTORY_VERSION = 0;

class Factory {
public:
	static Infill * createInfill(EepromReader &aReader);
	static TimeRange * Factory::createTimeRange(Planner * aPlanner, EepromReader &aReader);
};

#endif /* PLANNER_FACTORY_H_ */
