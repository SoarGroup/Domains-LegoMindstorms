/*
 * SoarCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 *  SoarCommunicator
 *    Abstract base class to use for enabling communication with the SoarManager
 *    Handles commands sent by the Manager and forwards new status updates to it
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

  // Called when soar is reinitialized (init-agent is called)
  //   Use to remove any references to SMl objects
  virtual void reinit(){}

  // Called by the SoarManager to send a command to the robot
  //   Implementers should add a '^status complete' wme to the identifier when the command is done
	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

  // Called by the SoarManager before the input phase 
	virtual void inputPhaseCallback(){}

protected:
  SoarManager* soarManager;
};

#endif /* SOARCOMMUNICATION_H_ */
