/*
 * Ev3Communicator.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "comm/Ev3Communicator.h"

#include "Constants.h"

#include <iostream>
using namespace std;

// Ev3Communicator
Ev3Communicator::Ev3Communicator(Ev3Manager* manager)
  :ev3Manager(manager), active(true){

	pthread_create(&sendStatusThread, 0, &sendStatusThreadFunction, this);
}

Ev3Communicator::~Ev3Communicator(){
  active = false;
  pthread_join(sendStatusThread, NULL);
}

void* Ev3Communicator::sendStatusThreadFunction(void* arg){
	Ev3Communicator* ev3Comm = (Ev3Communicator*)arg;
	while(ev3Comm->active){
    ev3Comm->sendStatusMessage();
		usleep(1000000/EV3_SEND_STATUS_FPS);
	}
	return 0;
}


