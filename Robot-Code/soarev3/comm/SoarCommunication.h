/*
 * SoarCommunication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARCOMMUNICATION_H_
#define SOARCOMMUNICATION_H_

#include <string>

class SoarManager;

#include "sml_Client.h"

#include "CommStructs.h"

#include "TcpClient.h"

#include <ctime>

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class SoarCommunicator {
public:
	virtual ~SoarCommunicator(){}

	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

	virtual void updateSoar() = 0;
};


class RemoteSoarCommunicator : public SoarCommunicator, TcpClient{
public:
	RemoteSoarCommunicator(string server_ip);

	virtual ~RemoteSoarCommunicator();

  virtual bool start();

	void assignManager(SoarManager* manager){
		soarManager = manager;
	}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void updateSoar();

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

  long last_time;
  int num_packets;

	uint nextAck;
	CommandMap waitingCommands;
	IdentifierMap waitingIdentifiers;
	IdentifierSet finishedIdentifiers;

};


#endif /* SOARCOMMUNICATION_H_ */
