/*
 * Ev3Communication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "Ev3Communication.h"

#include "ev3/Ev3Manager.h"

#include "util/CommUtil.h"

#include <sys/time.h>

#include <iostream>
#include <sstream>
using namespace std;

// Ev3Communicator
Ev3Communicator::Ev3Communicator(Ev3Manager* manager)
  :ev3Manager(manager){

}

void* Ev3Communicator::sendStatusThreadFunction(void* arg){
	Ev3Communicator* ev3Comm = (Ev3Communicator*)arg;
  int num_packets = 0;
  long last_time = (long)time(0);
	while(ev3Comm->isConnected()){
    ev3Comm->sendStatusMessage();
    num_packets++;
    if (((long)time(0)) != last_time){
      //printf("Packets per second: %d\n", num_packets);
      last_time = (long)time(0);
      num_packets = 0;
    }
		usleep(1000000/EV3_SEND_STATUS_FPS);
	}
	return 0;
}

bool Ev3Communicator::start(){
	pthread_create(&sendStatusThread, 0, &sendStatusThreadFunction, this);
  return true;
}

void Ev3Communicator::stop(){
  pthread_join(sendStatusThread, NULL);
}


// RemoteEv3Communicator
RemoteEv3Communicator::RemoteEv3Communicator(Ev3Manager* manager)
: Ev3Communicator(manager), TcpServer(){
  setReceptionCallback(&RemoteEv3Communicator::receiveMessage, this);

	pthread_mutex_init(&mutex, 0);
}

RemoteEv3Communicator::~RemoteEv3Communicator(){
  pthread_mutex_destroy(&mutex);
}

bool RemoteEv3Communicator::start(){
	Ev3Communicator::start();
  return TcpServer::start();
}

void RemoteEv3Communicator::stop(){
  Ev3Communicator::stop();
  TcpServer::stop();
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

  timeval now;
  gettimeofday(&now, 0);
  buffer.push_back(now.tv_usec);

	buffer.push_back(acks.size());
	for(AckSetIt it = acks.begin(); it != acks.end(); it++){
		buffer.push_back(*it);
	}

	StatusList statuses;
	ev3Manager->writeStatus(statuses);

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


