/*
 * SoarCommunication.h
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

	virtual void updateSoar() = 0;

  virtual bool start(){}

  virtual void stop(){};

  virtual bool isConnected(){
    return true;
  }
protected:
  SoarManager* soarManager;
};


class RemoteSoarCommunicator : public SoarCommunicator, public TcpClient{
public:
	RemoteSoarCommunicator(SoarManager* manager, string server_ip);

	virtual ~RemoteSoarCommunicator();

  bool openConnection();

  void closeConnection();

  virtual bool isConnected(){
    return TcpClient::isConnected();
  }

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
