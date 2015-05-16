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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
using namespace std;

using namespace sml;

// RemoteSoarCommunicator
RemoteSoarCommunicator::RemoteSoarCommunicator(string server_ip)
: soarManager(0), nextAck(1), server_ip(server_ip){
  socket_ready = setup_sockets();

	pthread_mutex_init(&mutex, 0);
}

bool RemoteSoarCommunicator::setup_sockets(){
  printf("creating socket\n");
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0){
    perror("ERROR: socket()");
    return false;
  }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	server_addr.sin_port = htons(7667);

  printf("connecting to server\n");
  if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    perror("ERROR: connect");
    return false;
  }

  printf("setup_sockets is success\n");

  return true;
}

void RemoteSoarCommunicator::start(){
	pthread_create(&sendThread, 0, &sendThreadFunction, this);
	pthread_create(&receiveThread, 0, &receiveThreadFunction, this);
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

      uchar* outBuffer;
      uint buf_size;
      packBuffer(buffer, outBuffer, buf_size);
      
      if (write(socket_fd, outBuffer, buf_size) < 0){
        perror("ERROR: write");
        delete [] outBuffer;
        pthread_mutex_lock(&mutex);
        break;
      }

      delete [] outBuffer;
    }

    pthread_mutex_unlock(&mutex);
		usleep(1000000/SOAR_SEND_COMMAND_FPS);
	}

	return 0;
}

void* RemoteSoarCommunicator::receiveThreadFunction(void* arg){
	RemoteSoarCommunicator* soarComm = (RemoteSoarCommunicator*)arg;
	while(true){
    uchar* 

    IntBuffer params;
    uint offset = 0;
    unpackBuffer((const uchar*)buf, buf_len, params);

    // Handle 1 message at a time
	  pthread_mutex_lock(&mutex);

    int messageType = params[offset++];
    if(messageType == ACK_MESSAGE){
      comm->receiveAckMessage(params, offset);
    } else if(messageType == STATUS_MESSAGE){
      comm->receiveStatusMessage(params, offset);
    }

	  pthread_mutex_unlock(&mutex);
	}
	return 0;

}

void RemoteSoarCommunicator::receiveAckMessage(IntBuffer& buffer, uint& offset){
	//cout << "--> Soar Receive Ack" << endl;

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

	//cout << "<-- Soar Receive Ack" << endl;
}

void RemoteSoarCommunicator::receiveStatusMessage(IntBuffer& buffer, uint& offset){
	//cout << "--> Soar Receive Status" << endl;
	StatusList statuses;
	uint numStatuses = buffer[offset++];
	for(uint i = 0; i < numStatuses; i++){
		statuses.push_back(Ev3Status(buffer, offset));
	}
	soarManager->readStatus(statuses);
	//cout << "<-- Soar Receive Status" << endl;
}

void RemoteSoarCommunicator::updateSoar(){
	pthread_mutex_lock(&mutex);
	for(IdentifierSet::iterator i = finishedIdentifiers.begin(); i != finishedIdentifiers.end(); i++){
		(*i)->CreateStringWME("status", "success");
	}
	finishedIdentifiers.clear();
	pthread_mutex_unlock(&mutex);
}

