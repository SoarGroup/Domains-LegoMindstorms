/*
 * Ev3Manager.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 *
 * Ev3Manager : CommandHandler
 *   Responsible for creating the different devices (brick, inputs, outputs)
 *   and connecting them to the communication layer
 */

#ifndef EV3MANAGER_H_
#define EV3MANAGER_H_

#include "Constants.h"
#include "comm/CommStructs.h"

#include "Ev3Brick.h"
#include "OutputDeviceManager.h"
#include "InputDeviceManager.h"

#include <map>
typedef std::map<int, CommandHandler*> CommandHandlers;
typedef CommandHandlers::iterator CommandHandlersIt;

class Ev3Manager : public CommandHandler {
public:
	Ev3Manager();

	virtual ~Ev3Manager();

  // Fills the given status list with statuses from each device
	void writeStatus(StatusList& statuses);

  // forwards the command to the correct device
	void executeEv3Command(Ev3Command* command);

private:
  // Writes any status info from the manager itself
	Ev3Status getStatus();

  // Handles commands directed towards the Ev3Manager itself
  // (none right now)
	void handleCommand(Ev3Command* command);

	Ev3Brick* brick;
	InputDeviceManager* inputDevices;
	OutputDeviceManager* outputDevices;

	CommandHandlers commandHandlers;
};

#endif /* EV3MANAGER_H_ */
