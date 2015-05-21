/*
 * Ev3Communicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef EV3COMMUNICATION_H_
#define EV3COMMUNICATION_H_

class Ev3Manager;

#include <pthread.h>

class Ev3Communicator{
public:
  Ev3Communicator(Ev3Manager* manager);

	virtual ~Ev3Communicator();

  void shutdown(){
    active = false;
  }

	static void* sendStatusThreadFunction(void*);

	virtual void sendStatusMessage() = 0;

protected:
  Ev3Manager* ev3Manager;

  bool active;

	pthread_t sendStatusThread;
};


#endif /* EV3COMMUNICATION_H_ */
