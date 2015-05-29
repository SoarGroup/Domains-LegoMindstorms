/*
 * Ev3Brick.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 * Ev3Brick : CommandHandler
 *   Interface to the hardware for the brick itself
 *   Reads button presses and sends led commands
 */

#ifndef EV3BRICK_H_
#define EV3BRICK_H_

#include "comm/CommStructs.h"

#include "lms2012.h"

class Ev3Brick : public CommandHandler {
public:
	Ev3Brick();
	virtual ~Ev3Brick();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int uiFile; // File for writing led commands
	UI* uiData; // Data map to read button info
};


#endif /* EV3BRICK_H_ */
