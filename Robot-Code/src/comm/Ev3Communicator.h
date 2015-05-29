/*
 * Ev3Communicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 *  Ev3Communicator
 *    Abstract base class to use for enabling communication with the Ev3Manager
 *    Periodically sends status updates to soar and sends commands to the Ev3Manager
 */

#ifndef EV3COMMUNICATION_H_
#define EV3COMMUNICATION_H_

class Ev3Manager;

#include <pthread.h>

class Ev3Communicator{
public:
  Ev3Communicator(Ev3Manager* manager);

	virtual ~Ev3Communicator();

  // Will cause the Ev3Communicator to stop sending status updates
  void shutdown(){
    active = false;
  }

  // Function called by the new send status thread
  // Will periodically call sendStatusMessage
  // (According to EV3_SEND_STATUS_FPS in Constants.h)
	static void* sendStatusThreadFunction(void*);

  // Implementing classes will get a status from the Ev3Manager
  //   and send it to the SoarManager
	virtual void sendStatusMessage() = 0;

protected:
  Ev3Manager* ev3Manager;

  bool active; // true if sending status updates

	pthread_t sendStatusThread;
};


#endif /* EV3COMMUNICATION_H_ */
