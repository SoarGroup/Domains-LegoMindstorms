/*
 * SoarCommunicator.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARCOMMUNICATION_H_
#define SOARCOMMUNICATION_H_

#include "comm/TcpClient.h"

#include <string>

class SoarManager;

#include "sml_Client.h"

#include "CommStructs.h"

#include <ctime>

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class SoarCommunicator {
public:
  SoarCommunicator(SoarManager* manager);

	virtual ~SoarCommunicator(){}

	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

	virtual void inputPhaseCallback() = 0;

protected:
  SoarManager* soarManager;
};


class RemoteSoarCommunicator : public SoarCommunicator, public TcpClient{
public:
	RemoteSoarCommunicator(SoarManager* manager, string server_ip);

	virtual ~RemoteSoarCommunicator();

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void inputPhaseCallback();

private:
  static void* sendThreadFunction(void* arg);

  void sendCommands();

  static void receiveMessage(const void* buffer, int buf_len, void* user);

	void receiveAckMessage(IntBuffer& buffer, uint& offset);

	void receiveStatusMessage(IntBuffer& buffer, uint& offset);

private:
  pthread_t sendThread;
	pthread_mutex_t mutex;

	SoarManager* soarManager;

	uint nextAck;
	CommandMap waitingCommands;
	IdentifierMap waitingIdentifiers;
	IdentifierSet finishedIdentifiers;

};


#endif /* SOARCOMMUNICATION_H_ */
