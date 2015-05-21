/*
 * SoarCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOAR_COMMUNICATOR_H_
#define SOAR_COMMUNICATOR_H_

class SoarManager;

#include "comm/CommStructs.h"

#include "sml_Client.h"

class SoarCommunicator {
public:
  SoarCommunicator(SoarManager* manager)
    :soarManager(manager){}

	virtual ~SoarCommunicator(){}

	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

	virtual void inputPhaseCallback() = 0;

protected:
  SoarManager* soarManager;
};

#endif /* SOARCOMMUNICATION_H_ */
