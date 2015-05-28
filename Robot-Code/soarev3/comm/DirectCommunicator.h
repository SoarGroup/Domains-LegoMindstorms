/*
 * DirectCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 * DirectCommunicator : Ev3Communicator, SoarCommunicator
 *   Used when the SoarManager and Ev3Manager are both running on the robot
 *   Directly forwards messages (statuses + commands) between the two
 *     without using a networking connection
 */

#ifndef DIRECTCOMMUNICATION_H_
#define DIRECTCOMMUNICATION_H_

#include "comm/Ev3Communicator.h"
#include "comm/SoarCommunicator.h"

#include "sml_Client.h"

#include <ostream>

class DirectCommunicator : public Ev3Communicator, public SoarCommunicator {
public:
	DirectCommunicator(SoarManager* sm, Ev3Manager* em);

	virtual ~DirectCommunicator(){}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void sendStatusMessage();
};


#endif /* DIRECTCOMMUNICATION_H_ */
