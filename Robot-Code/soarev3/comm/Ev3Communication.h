/*
 * Ev3Communication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef EV3COMMUNICATION_H_
#define EV3COMMUNICATION_H_

class Ev3Manager;

#include "CommStructs.h"

#include <pthread.h>

#include <string>

#include "TcpServer.h"

#include <ctime>

class Ev3Communicator{
public:
	virtual ~Ev3Communicator(){}

	virtual bool start();

	static void* sendStatusThreadFunction(void*);

	virtual void sendStatusMessage() = 0;

protected:
	pthread_t sendStatusThread;
};


class RemoteEv3Communicator : public Ev3Communicator, public TcpServer{
public:
	RemoteEv3Communicator();

	virtual ~RemoteEv3Communicator();

	void assignManager(Ev3Manager* manager){
		ev3Manager = manager;
	}

	virtual bool start();

private:
  static void receiveMessage(const void* buffer, int buf_len, void* user);

	void receiveCommandMessage(IntBuffer& buffer, uint& offset);

	void sendStatusMessage();

private:
	Ev3Manager* ev3Manager;

	AckSet acks;
	pthread_mutex_t mutex;
};

#endif /* EV3COMMUNICATION_H_ */
