/*
 * Controller.h
 *
 *  Created on: 29 июл. 2018 г.
 *      Author: Andrey
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Controller {
public:
	virtual void run() = 0;
	virtual void enable(bool aEnabled) = 0;
	virtual ~Controller() {};
};

#endif /* CONTROLLER_H_ */
