/*
 * StrToNum.h
 *
 *  Created on: 19 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef COMMAND_STRTONUM_H_
#define COMMAND_STRTONUM_H_

template<typename Num>
class StrToNum {
public:
	Num _result;
	bool set(char * aStrNum);
};

template<typename Num>
inline bool StrToNum<Num>::set(char* aStrNum) {
	Num res = 0;
	if (!aStrNum) {
		return false;
	}

	for(int i = 0; aStrNum[i]; i++) {
		char ch = aStrNum[i];
		if (ch > '9' || ch < '0') {
			return false;
		}
		res = res * 10 + ch - '0';
	}

	_result = res;
	return true;
}

#endif /* COMMAND_STRTONUM_H_ */
