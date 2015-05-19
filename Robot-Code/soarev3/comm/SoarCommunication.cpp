/*
 * SoarCommunication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "SoarCommunication.h"


#include "Constants.h"
#include "CommStructs.h"

#include "soar/SoarManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using namespace std;
using namespace sml;

#ifdef DEBUG_NETWORKING
#include <sys/types.h>
#include <ctime>
#endif

// RemoteSoarCommunicator
RemoteSoarCommunicator::RemoteSoarCommunicator(string server_ip)
: TcpClient(server_ip), soarManager(0), nextAck(1){
	pthread_mutex_init(&mutex, 0);
  this->setReceptionCallback(&RemoteSoarCommunicator::receiveMessage, this);
}

RemoteSoarCommunicator::~RemoteSoarCommunicator(){
  pthread_mutex_destroy(&mutex);
}

bool RemoteSoarCommunicator::start(){
	pthread_create(&sendThread, 0, &sendThreadFunction, this);
  return TcpClient::start();
}

void RemoteSoarCommunicator::sendCommandToEv3(Ev3Command command, Identifier* id){
	pthread_mutex_lock(&mutex);
	// Need to send the command over LCM and wait for the ack
	uint ack = nextAck++;
	command.ack = ack;
	waitingCommands[ack] = command;
	waitingIdentifiers[ack] = id;
	pthread_mutex_unlock(&mutex);
}

void* RemoteSoarCommunicator::sendThreadFunction(void* arg){
	RemoteSoarCommunicator* soarComm = (RemoteSoarCommunicator*)arg;

	while(true){
    if (soarComm->isConnected()){
      soarComm->sendCommands();
    }
    usleep(1000000/SOAR_SEND_COMMAND_FPS);
  }

	return 0;
}

void RemoteSoarCommunicator::sendCommands(){
  pthread_mutex_lock(&mutex);
  
  // Send Message to Ev3 of all queued commands
  // Add Commands to the message
  if(waitingCommands.size() > 0){
    IntBuffer buffer;
    buffer.push_back(COMMAND_MESSAGE);
    buffer.push_back(waitingCommands.size());

    for(CommandMapIt i = waitingCommands.begin(); i != waitingCommands.end(); i++){
      i->second.packCommand(buffer);
    }

    char* outBuffer;
    uint buf_size;
    packBuffer(buffer, outBuffer, buf_size);
    sendPacket(outBuffer, buf_size);
    delete [] outBuffer;
  }

  pthread_mutex_unlock(&mutex);
}

void RemoteSoarCommunicator::receiveMessage(const void* buffer, int buf_len, void* user){
  RemoteSoarCommunicator* comm = (RemoteSoarCommunicator*)user;

  IntBuffer params;
  uint offset = 0;
  unpackBuffer((const char*)buffer, buf_len, params);

  // Handle 1 message at a time
  comm->receiveStatusMessage(params, offset);

  long now = (long)time(0);
  if (now == comm->last_time){
    comm->num_packets++;
  } else {
    //printf("Packets per second: %d\n", (comm->num_packets + 1));
    comm->num_packets = 1;
  }
  comm->last_time = now;
}

void RemoteSoarCommunicator::receiveStatusMessage(IntBuffer& buffer, uint& offset){
	//cout << "--> Soar Receive Status" << endl;
  pthread_mutex_lock(&mutex);
  //

  int send_time = buffer[offset++];
  timeval now;
  gettimeofday(&now, 0);
  int dt = now.tv_usec - send_time;
  if(dt < 0){
    dt += 1000000;
  }
  //printf("LATENCY: %d\n", dt);

	uint numAcks = buffer[offset++];
	for(uint i = 0; i < numAcks; i++){
		uint ack = buffer[offset++];
		CommandMapIt it = waitingCommands.find(ack);
		if(it != waitingCommands.end()){
			waitingCommands.erase(it);
		}
		IdentifierMap::iterator jt = waitingIdentifiers.find(ack);
		if(jt != waitingIdentifiers.end()){
			//cout << "Added "<< ack << " to finished ids " << jt->second->GetValueAsString() << endl;
			finishedIdentifiers.insert(jt->second);
			waitingIdentifiers.erase(jt);
		}
	}

	StatusList statuses;
	uint numStatuses = buffer[offset++];
	for(uint i = 0; i < numStatuses; i++){
		statuses.push_back(Ev3Status(buffer, offset));
	}
	soarManager->readStatus(statuses);

	//cout << "<-- Soar Receive Status" << endl;
  pthread_mutex_unlock(&mutex);
}

void RemoteSoarCommunicator::updateSoar(){
	pthread_mutex_lock(&mutex);

	for(IdentifierSet::iterator i = finishedIdentifiers.begin(); i != finishedIdentifiers.end(); i++){
		(*i)->CreateStringWME("status", "success");
	}
	finishedIdentifiers.clear();
  
	pthread_mutex_unlock(&mutex);
}
