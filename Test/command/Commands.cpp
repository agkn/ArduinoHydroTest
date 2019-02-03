/*
 * Commands.cpp
 *
 *  Created on: 18 авг. 2018 г.
 *      Author: Andrey
 */

#include "Commands.h"
#include "Commander.h"
#include "../debug.h"
#include "StrToNum.h"
#include "../RTC.h"

int ListJobs::onProcess(const char aCmd, char** aArgV, int aArcC) {
	dout("List of jobs");

	return Commander::RES_OK;
}

int ListTimePins::onProcess(const char aCmd, char** aArgV, int aArcC) {
	dout("List of time pins");

	return Commander::RES_OK;
}

int ListResources::onProcess(const char aCmd, char** aArgV, int aArcC) {
	dout("List of resources");

	return Commander::RES_OK;
}

int SetTimePin::onProcess(const char aCmd, char** aArgV, int aArcC) {
	StrToNum<uint8_t> converter;

	dout("Set pin: %s:%s:%s", aArgV[0], aArgV[1], aArgV[2]);

	if (!converter.set(aArgV[0])) {
		dout("Can't parse id %s", aArgV[0]);
		return Commander::RES_BAD_ARGUMENT;
	}

	uint8_t id = converter._result;

	if (!converter.set(aArgV[1])) {
		dout("Can't parse hour %s", aArgV[1]);
		return Commander::RES_BAD_ARGUMENT;
	}

	dout("minutes %d", converter._result);
	uint32_t seconds = 3600l * converter._result;

	if (!converter.set(aArgV[2])) {
		dout("Can't parse minutes %s", aArgV[2]);
		return Commander::RES_BAD_ARGUMENT;
	}

	seconds += 60l * converter._result;

	if (seconds >= 24l * 3600) {
		dout("Time out of range %s:%s. %lu > 86400", aArgV[1], aArgV[2],
				seconds);
		return Commander::RES_BAD_ARGUMENT;
	}

	dout("Set pin[%d]: %lu", id, seconds);

	return mCommander->getPlanner()->setTimePin(id, seconds) ?
			Commander::RES_OK : Commander::RES_BAD_ARGUMENT;
}

int TimeCommand::process(const char aCmd, char** aArgV, int aArcC) {
	if (CMD_TIME != aCmd) {
		return Commander::RES_BAD_COMMAND;
	}

	if (aArcC == 0) {
		Hydra::DateTime now = this->mCommander->getRtc()->now();

		this->mCommander->out(CMD_TIME)->p(now.year)->p(now.month)->p(now.day)->p(now.hour)->p(now.minutes)->p(now.seconds)->send();
		return Commander::RES_OK;

	} else if (aArcC == 6) {
		StrToNum<uint16_t> converter;

		if (!converter.set(aArgV[0])) {
			dout("Can't parse year %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}
		uint16_t year = converter._result;

		if (!converter.set(aArgV[1])) {
			dout("Can't parse month %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}
		uint16_t month = converter._result;

		if (!converter.set(aArgV[2])) {
			dout("Can't parse day %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}
		uint16_t day = converter._result;

		if (!converter.set(aArgV[3])) {
			dout("Can't parse hour %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}
		uint16_t hour = converter._result;

		if (!converter.set(aArgV[4])) {
			dout("Can't parse minutes %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}
		uint16_t minites = converter._result;

		if (!converter.set(aArgV[5])) {
			dout("Can't parse seconds %s", aArgV[2]);
			return Commander::RES_BAD_ARGUMENT;
		}

		this->mCommander->getRtc()->setTime(year, month, day,
				hour, minites, converter._result);

		return Commander::RES_OK;
	}

	return Commander::RES_BAD_COMMAND;
}
