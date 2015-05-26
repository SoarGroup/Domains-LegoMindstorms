/*
 * Ev3Communicator.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */


#include "comm/RemoteEv3Communicator.h"

#include "ev3/Ev3Manager.h"

#include "util/CommUtil.h"

#include <iostream>
#include <sstream>
using namespace std;


// RemoteEv3Communicator
RemoteEv3Communicator::RemoteEv3Communicator(Ev3Manager* manager)
: Ev3Communicator(manager), TcpServer(){
  setReceptionCallback(&RemoteEv3Communicator::receiveMessage, this);

	pthread_mutex_init(&mutex, 0);
}

RemoteEv3Communicator::~RemoteEv3Communicator(){
  pthread_mutex_destroy(&mutex);
}

void RemoteEv3Communicator::receiveMessage(const void* buffer, int buf_len, void* user){
	RemoteEv3Communicator* comm = (RemoteEv3Communicator*)user;

	IntBuffer params;
	uint offset = 0;
	unpackBuffer((const char*)buffer, buf_len, params);

	uint messageType = params[offset++];
	if(messageType == COMMAND_MESSAGE){
		comm->receiveCommandMessage(params, offset);
	}
}

void RemoteEv3Communicator::receiveCommandMessage(IntBuffer& buffer, uint& offset){
	pthread_mutex_lock(&mutex);
	//cout << "--> Ev3 Receive Command" << endl;

	// finishedAcks - acks that we stopped receiving (processed)
	AckSet finishedAcks;
	finishedAcks.insert(acks.begin(), acks.end());

	uint numCommands = buffer[offset++];
	for(uint i = 0; i < numCommands; i++){
		Ev3Command command;
		command.unpackCommand(buffer, offset);
		if(acks.find(command.ack) == acks.end()){
			// New command, execute
			ev3Manager->executeEv3Command(&command);
			acks.insert(command.ack);
		}
		finishedAcks.erase(command.ack);
	}

	for(AckSetIt i = finishedAcks.begin(); i != finishedAcks.end(); i++){
		acks.erase(*i);
	}
	//cout << "<-- Ev3 Receive Command" << endl;
	pthread_mutex_unlock(&mutex);
}


void RemoteEv3Communicator::sendStatusMessage(){
  if (!this->isConnected()){
    return;
  }
	pthread_mutex_lock(&mutex);
	//cout << "--> Ev3 Send Status" << endl;
	IntBuffer buffer;

  //printf("Sending %d acks\n", acks.size());

	buffer.push_back(acks.size());
	for(AckSetIt it = acks.begin(); it != acks.end(); it++){
		buffer.push_back(*it);
	}

	StatusList statuses;
	ev3Manager->writeStatus(statuses);

  //printf("Sending %d statuses\n", statuses.size());

	buffer.push_back(statuses.size());
	for(uint i = 0; i < statuses.size(); i++){
		statuses[i].packStatus(buffer);
	}

  buffer.push_back(10);

	// Create + send outgoing buffer
	char* outBuffer;
	uint buf_size;
	packBuffer(buffer, outBuffer, buf_size);
  //printf("Sending packet of size %d\n", buf_size);
  sendPacket(outBuffer, buf_size);
	delete [] outBuffer;
	//cout << "<-- Ev3 Send Status" << endl;
	pthread_mutex_unlock(&mutex);
}


