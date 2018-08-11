/*
 * Factory.cpp
 *
 *  Created on: 11 авг. 2018 г.
 *      Author: Andrey
 */

#include "Factory.h"

static Infill * Factory::createInfill(EepromReader &aReader) {
	switch(aReader.readU8()) {
	case Infill::Type::ConstantFill: return new ConstantFill(FACTORY_VERSION, aReader);
	case Infill::Type::DrainFloodFill: return new DrainFloodFill(FACTORY_VERSION, aReader);
	}

	return NULL;
}

static TimeRange * Factory::createTimeRange(Planner * aPlanner, EepromReader &aReader) {
	return new TimeRange(aPlanner, FACTORY_VERSION, aReader);
}
