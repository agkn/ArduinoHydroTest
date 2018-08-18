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
	ListJobs(Commander * aCommaner): SimpleCommand(aCommaner, 'j', 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class ListTimePins: public SimpleCommand {
public:
	ListTimePins(Commander * aCommaner): SimpleCommand(aCommaner, 't', 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class ListResources: public SimpleCommand {
public:
	ListResources(Commander * aCommaner): SimpleCommand(aCommaner, 'r', 0) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};

class SetTimePin: public SimpleCommand {
public:
	SetTimePin(Commander * aCommaner): SimpleCommand(aCommaner, 't', 3) {}

	virtual int onProcess(const char aCmd, char **aArgV, int aArcC);
};


#endif /* COMMAND_COMMANDS_H_ */
