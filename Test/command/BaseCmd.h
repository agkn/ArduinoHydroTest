/*
 * BaseCmd.h
 *
 *  Created on: 16 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMAND_BASECMD_H_
#define COMMAND_BASECMD_H_

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
