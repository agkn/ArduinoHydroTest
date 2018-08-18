/*
 * SimpleCommand.cpp
 *
 *  Created on: 18 авг. 2018 г.
 *      Author: Andrey
 */

#include "SimpleCommand.h"
#include "Commander.h"

int SimpleCommand::process(const char aCmd, char** aArgV, int aArgC) {
	if (mCmd != aCmd || aArgC != this->mArgNumber) {
		return Commander::RES_BAD_COMMAND;
	}

	return this->onProcess(aCmd, aArgV, aArgC);
}
