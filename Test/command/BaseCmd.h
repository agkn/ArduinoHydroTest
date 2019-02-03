/*
 * BaseCmd.h
 *
 *  Created on: 16 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMAND_BASECMD_H_
#define COMMAND_BASECMD_H_

const char CMD_TIME 		= 't';
const char CMD_QUERY_RES 	= 'j';
const char CMD_QUERY_JOB 	= 'j';
const char CMD_ADD_JOB 		= 'j';
const char CMD_QUERY_PINS	= 'p';
const char CMD_SET_PIN		= 'p';
const char CMD_REMOVE_JOB	= 'r';

const char ARG_SEPARATOR = ':';

class Commander;

class BaseCmd {
	friend class Commander;
	BaseCmd * mNext;

protected:
	Commander * mCommander;

public:
	BaseCmd(Commander *aCommander): mCommander(aCommander), mNext(0) {};

	virtual int process(const char aCmd, char **aArgV, int aArcC) = 0;

	virtual ~BaseCmd() {};

	BaseCmd * addNext(BaseCmd * aNextCmd) {
		aNextCmd->mNext = mNext;
		mNext = aNextCmd;
		return this;
	}
};

#endif /* COMMAND_BASECMD_H_ */
