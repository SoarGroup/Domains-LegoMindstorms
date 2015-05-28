/*
 * InputDeviceManager.h
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 *
 * InputDeviceManager : CommandHandler
 *   Reads information from the input devices (sensors)
 *    and handles commands to set sensor modes
 *   Handles both serial and analog connections
 *   Analog ones have to be explictly created by a command (can't autodetect)
 */

#ifndef INPUTDEVICEMANAGER_H_
#define INPUTDEVICEMANAGER_H_

#include "lms2012.h"

#include "comm/CommStructs.h"

class InputDeviceManager : public CommandHandler {
public:
	InputDeviceManager();
	void initDevices();
	virtual ~InputDeviceManager();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int uartFile;  // File handle for writing commands (change mode)
	UART* uart;    // Data map for getting information about serial sensors

	int analogFile; // File handle for writing commands to analog devices (change mode)
	ANALOG* analog; // Data map for getting informatino about analog sensors
	uchar analogDevs[NUM_INPUTS]; // Information about which analog devices are connected 

	DEVCON devcon;  // Data used to set mode stuff
};

#endif /* INPUTDEVICEMANAGER_H_ */
