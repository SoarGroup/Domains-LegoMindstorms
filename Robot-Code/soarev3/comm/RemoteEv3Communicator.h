/*
 * Ev3Communicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef EV3COMMUNICATION_H_
#define EV3COMMUNICATION_H_

#include "comm/Ev3Communicator.h"

#include <string>

#include "comm/TcpServer.h"

class RemoteEv3Communicator : public Ev3Communicator, public TcpServer{
public:
	RemoteEv3Communicator(Ev3Manager* manager);

	virtual ~RemoteEv3Communicator();

private:
  static void receiveMessage(const void* buffer, int buf_len, void* user);

	void receiveCommandMessage(IntBuffer& buffer, uint& offset);

	void sendStatusMessage();

private:
	AckSet acks;

	pthread_mutex_t mutex;
};

#endif /* EV3COMMUNICATION_H_ */
