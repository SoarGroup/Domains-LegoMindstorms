/*
 * RemoteEv3Communicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 * RemoteEv3Communicator : Ev3Communicator, TcpServer
 *   Enables the Ev3Manager to communicator over a TCP connection
 *   Acts as a server on the robot that a soar client can remotely connect to
 */

#ifndef REMOTE_EV3_COMMUNICATOR_H_
#define REMOTE_EV3_COMMUNICATOR_H_

#include "comm/Ev3Communicator.h"

#include "comm/TcpServer.h"

class RemoteEv3Communicator : public Ev3Communicator, public TcpServer{
public:
	RemoteEv3Communicator(Ev3Manager* manager);

	virtual ~RemoteEv3Communicator();

private:
  // callback when a message from the soar client is received
  static void receiveMessage(const void* buffer, int buf_len, void* user);

  // handles a message containing commands for the robot
  //   starting at the given offset
	void receiveCommandMessage(IntBuffer& buffer, uint& offset);

  // Gets status information from the Ev3Manager and sends it to the client
  //   in a packet
	void sendStatusMessage();

private:
  // A set of acks for commands successfully executed
	AckSet acks;

	pthread_mutex_t mutex;
};

#endif /* REMOTE_EV3_COMMUNICATOR_H_ */
