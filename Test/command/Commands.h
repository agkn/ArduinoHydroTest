/*
 * Commands.h
 *
 *  Created on: 18 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMAND_COMMANDS_H_
#define COMMAND_COMMANDS_H_
#include "SimpleCommand.h"

class ListJobs: public SimpleCommand {
public:
	ListJobs(Commander * aCommaner): SimpleCommand(aCommaner, CMD_QUERY_JOB, 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class ListTimePins: public SimpleCommand {
public:
	ListTimePins(Commander * aCommaner): SimpleCommand(aCommaner, CMD_QUERY_PINS, 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class ListResources: public SimpleCommand {
public:
	ListResources(Commander * aCommaner): SimpleCommand(aCommaner, CMD_QUERY_RES, 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class SetTimePin: public SimpleCommand {
public:
	SetTimePin(Commander * aCommaner): SimpleCommand(aCommaner, CMD_SET_PIN, 3) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class TimeCommand: public BaseCmd {
public:
	TimeCommand(Commander * aCommaner): BaseCmd(aCommaner) {}

	virtual int process(const char aCmd, char **aArgV, int aArcC);
};


#endif /* COMMAND_COMMANDS_H_ */
