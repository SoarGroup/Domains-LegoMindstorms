/*
 * DirectCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef DIRECTCOMMUNICATION_H_
#define DIRECTCOMMUNICATION_H_

#include "Ev3Communicator.h"
#include "SoarCommunicator.h"

#include "sml_Client.h"

class DirectCommunicator : public Ev3Communicator, SoarCommunicator {
public:
	DirectCommunicator(SoarManager* sm, Ev3Manager* em);

	virtual ~DirectCommunicator(){}

  virtual void start();

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void sendStatusMessage();

	void updateSoar(){}
};


#endif /* DIRECTCOMMUNICATION_H_ */
