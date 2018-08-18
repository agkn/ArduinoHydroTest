/*
 * Commands.cpp
 *
 *  Created on: 18 авг. 2018 г.
 *      Author: Andrey
 */

#include "Commands.h"
#include "Commander.h"
#include "../debug.h"

int ListJobs::onProcess(const char aCmd, char** aArgV, int aArcC) {
	mCommander->out("List of jobs");

	return Commander::RES_OK;
}

int ListTimePins::onProcess(const char aCmd, char** aArgV, int aArcC) {
	mCommander->out("List of time pins");

	return Commander::RES_OK;
}

int ListResources::onProcess(const char aCmd, char** aArgV, int aArcC) {
	mCommander->out("List of resources");

	return Commander::RES_OK;
}

int SetTimePin::onProcess(const char aCmd, char** aArgV, int aArcC) {
	mCommander->out("Set time pin");
	dout("Set pin: %s:%s:%s", aArgV[0], aArgV[1], aArgV[2]);

	return Commander::RES_OK;
}
