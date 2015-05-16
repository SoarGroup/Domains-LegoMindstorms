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

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class SoarCommunicator {
public:
	virtual ~SoarCommunicator(){}

	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

	virtual void updateSoar() = 0;
};


class RemoteSoarCommunicator : public SoarCommunicator{
public:
	RemoteSoarCommunicator(string server_ip);

	virtual ~SoarCommunicator(){
		pthread_mutex_destroy(&mutex);
    if (socket_ready){
      close(socket_fd);
    }
	}

	void assignManager(SoarManager* manager){
		soarManager = manager;
	}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void start();

	void updateSoar();

private:
  bool setup_sockets();

  static void* sendThreadFunction(void* arg);

  static void* receiveThreadFunction(void* arg);

	void receiveAckMessage(IntBuffer& buffer, uint& offset);

	void receiveStatusMessage(IntBuffer& buffer, uint& offset);

private:
  string server_ip;
  int socket_fd;
  bool socket_ready;

  pthread_t sendThread;
  pthread_t receiveThread;
	pthread_mutex_t mutex;

	SoarManager* soarManager;

	uint nextAck;
	CommandMap waitingCommands;
	IdentifierMap waitingIdentifiers;
	IdentifierSet finishedIdentifiers;

};


#endif /* SOARCOMMUNICATION_H_ */
