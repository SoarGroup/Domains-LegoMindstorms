/*
 * SoarCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 * RemoteSoarCommunicator : SoarCommunicator, TcpClient
 *   Enables the SoarManager to communicator over a TCP connection with the robot
 *   Connects to a server running on the robot
 */

#ifndef _REMOTE_SOAR_COMMUNICATOR_H_
#define _REMOTE_SOAR_COMMUNICATOR_H_

#include "comm/SoarCommunicator.h"
#include "comm/TcpClient.h"

#include <pthread.h>

#include <string>
#include <set>
#include <map>

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class RemoteSoarCommunicator : public SoarCommunicator, public TcpClient{
public:
	RemoteSoarCommunicator(SoarManager* manager, string server_ip);

	virtual ~RemoteSoarCommunicator();

  // Called when 'init-agent' is called 
  //   Removes all references to identifiers
  void reinit();

  // Queues up the given command to be sent to the robot
	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

  // Used to mark identifiers for waiting commands as complete
	void inputPhaseCallback();

private:
  // Thread function that periodically calls sendCommands
  //   (According to SOAR_SEND_COMMAND_FPS in Constants.h)
  static void* sendThreadFunction(void* arg);

  // Sends a list of all outstanding commands to the robot over the connection
  void sendCommands();

  // Callback when a packet is received
  static void receiveMessage(const void* buffer, int buf_len, void* user);

  // Called if a message contains a status update
	void receiveStatusMessage(IntBuffer& buffer, uint& offset);

private:
  pthread_t sendThread;
	pthread_mutex_t mutex;

	uint nextAck; // Used to uniquely assign ack id's
	CommandMap waitingCommands; // Set of outstanding commands we have not received an ack for yet
	IdentifierMap waitingIdentifiers; // Identifiers belonging to commands we are waiting on an ack for
	IdentifierSet finishedIdentifiers; // Identifiers belonging to commands we have received an ack for 
};


#endif /* _REMOTE_SOAR_COMMUNICATOR_H_ */
