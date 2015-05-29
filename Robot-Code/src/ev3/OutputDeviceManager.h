/*
 * OutputDeviceManager.h
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 *
 * OutputDeviceManager : CommandHandler
 *   Handles reading information about output devices (motors)
 *     And sending commands to them (e.g. set speed)
 *
 *
 */

#ifndef OUTPUTDEVICEMANAGER_H_
#define OUTPUTDEVICEMANAGER_H_

#include "comm/CommStructs.h"
#include "util/CommUtil.h"

#include "lms2012.h"

class OutputDeviceManager : public CommandHandler {
public:
	OutputDeviceManager();
	virtual ~OutputDeviceManager();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int motorInputFile;   // file to get motor info from 
	int motorOutputFile;  // file to send motor commands to
	MOTORDATA* motorData; // data map used for getting info about motors
};

#endif /* OUTPUTDEVICEMANAGER_H_ */
