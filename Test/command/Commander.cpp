/*
 * Commander.cpp
 *
 *  Created on: 13 авг. 2018 г.
 *      Author: Andrey
 */

#include "Commander.h"
#include "../debug.h"
#include "Commands.h"

const char CMD_SET_TIME_PIN = "t";
const char CMD_ADD_JOB 		= "j";
const char CMD_DEL_JOB 		= "d";
const char CMD_ADD_TIME_PIN = "t";

const char ARG_SEPARATOR = ':';

const int CMD_BUFF_LEN = 12;
const int CMD_ARGS = 10;

char mBuffer[CMD_BUFF_LEN];
char *mArgs[CMD_ARGS];
int mArgNum;
int mCharIndex;


void Commander::init(Stream& aStream) {
	mStream = &aStream;
}

void Commander::run() {
	while(mStream->available()) {
		int ch = mStream->read();
		dout("Symbol %d", ch);
		if (ch > 0) {
			if (mCharIndex < CMD_BUFF_LEN) {
				if (iscntrl(ch)) { // end of command.
					//dout("EOC found at %d", mCharIndex);
					if (mCharIndex > 0) {
						mBuffer[mCharIndex] = '\0';
						proccessCommand();
					} else {
						continue;
					}
				} else {
					mBuffer[mCharIndex++] = ch;
				}
			} else { // mCharIndex < CMD_BUFF_LEN
				dout("Overflow");
				if (iscntrl(ch)) {
					dout("Overflow: Finish");
					fail(RES_TOO_LONG);
				}
			}
		} // ch > 0
	}
}

void Commander::proccessCommand() {
	int argc = 0;

	for(int i = 0; mBuffer[i] != '\0'; i++) {
//		dout("Check %c", mBuffer[i]);
		if (ARG_SEPARATOR == mBuffer[i]) {
			mArgs[argc++] = mBuffer + i + 1;
			mBuffer[i] = '\0';
		}
	}

	execute(mBuffer[0], mArgs, argc);
}

void Commander::execute(char aCmd, char **aArgv, int aArgC) {
	dout("Execute: %c, args: %d", aCmd, aArgC);

	int res = RES_BAD_COMMAND;
	for(BaseCmd * cmd = mCommands; NULL != cmd; cmd = cmd->mNext){
		res = cmd->process(aCmd, aArgv, aArgC);
		if (RES_BAD_COMMAND != res) {
			break;
		}
	}

	if (RES_OK == res ) {
		ok();
	} else {
		fail(res);
	}
}

void Commander::fail(int aResCode) {
	mStream->print("Fail: ");
	mStream->println(aResCode);
	reset();
}

void Commander::ok() {
	mStream->println("OK");
	reset();
}

void Commander::out(const char* aMessage) {
	mStream->println(aMessage);
}

void Commander::reset() {
	mArgNum = 0;
	mCharIndex = 0;
}

Commander::Commander(): mStream(NULL) {
	 mCommands = new ListJobs(this);
	 mCommands->addNext(new ListResources(this));
	 mCommands->addNext(new ListTimePins(this));
	 mCommands->addNext(new SetTimePin(this));
}
