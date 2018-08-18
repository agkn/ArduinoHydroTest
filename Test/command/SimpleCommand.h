/*
 * SimpleCommand.h
 *
 *  Created on: 18 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMAND_SIMPLECOMMAND_H_
#define COMMAND_SIMPLECOMMAND_H_
#include "BaseCmd.h"

class SimpleCommand: public BaseCmd {
	const char mCmd;
	const int mArgNumber;

public:

	SimpleCommand(Commander * aCommander, char aCmd, int aArgNumber): BaseCmd(aCommander), mCmd(aCmd), mArgNumber(aArgNumber) {};

	virtual int process(const char aCmd, char **aArgv, int aArcC);

	virtual int onProcess(const char aCmd, char **aArgv, int aArcC) = 0;
};

#endif /* COMMAND_SIMPLECOMMAND_H_ */
