/*
 * DirectCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef DIRECTCOMMUNICATION_H_
#define DIRECTCOMMUNICATION_H_

#include "comm/Ev3Communicator.h"
#include "comm/SoarCommunicator.h"

#include "sml_Client.h"

class DirectCommunicator : public Ev3Communicator, SoarCommunicator {
public:
	DirectCommunicator(SoarManager* sm, Ev3Manager* em);

	virtual ~DirectCommunicator(){}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void sendStatusMessage();

	void inputPhaseCallback(){}
};


#endif /* DIRECTCOMMUNICATION_H_ */
