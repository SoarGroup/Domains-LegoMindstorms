/*
 * DirectCommunicator.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "DirectCommunicator.h"

#include "soar/SoarManager.h"
#include "ev3/Ev3Manager.h"

using namespace sml;

DirectCommunicator::DirectCommunicator(SoarManager* sm, Ev3Manager* em)
: SoarManager(sm), Ev3Manager(em){
}

void DirectCommunicator::start(){
  Ev3Communicator::start();
}

void DirectCommunicator::sendStatusMessage(){
	StatusList statuses;
	ev3Manager->writeStatus(statuses);
	soarManager->readStatus(statuses);
}

void DirectCommunicator::sendCommandToEv3(Ev3Command command, Identifier* id){
	ev3Manager->executeEv3Command(&command);
	id->CreateStringWME("status", "complete");
}


