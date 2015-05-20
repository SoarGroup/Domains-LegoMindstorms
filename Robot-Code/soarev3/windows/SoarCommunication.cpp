/*
 * SoarCommunication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "windows/SoarCommunication.h"


#include "Constants.h"
#include "comm/CommStructs.h"

#include "soar/SoarManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
using namespace sml;

#ifdef DEBUG_NETWORKING
#include <sys/types.h>
#include <ctime>
#endif

// SoarCommunicator

SoarCommunicator::SoarCommunicator(SoarManager* manager)
	:soarManager(manager){

}

// RemoteSoarCommunicator

RemoteSoarCommunicator::RemoteSoarCommunicator(SoarManager* manager, string server_ip)
	: SoarCommunicator(manager), TcpClient(server_ip), nextAck(1){
    mutex = CreateMutex(NULL, FALSE, NULL);
    if(mutex == NULL){
        printf("CreateMutex error: %d\n", GetLastError());
    }
    this->setReceptionCallback(&RemoteSoarCommunicator::receiveMessage, this);
}

RemoteSoarCommunicator::~RemoteSoarCommunicator() {
    CloseHandle(mutex);
}

bool RemoteSoarCommunicator::openConnection() {
    DWORD threadId;
    sendThread = CreateThread(NULL, 0, &sendThreadFunction, (void*)this, 0, &threadId);
	return TcpClient::openConnection();
}

void RemoteSoarCommunicator::closeConnection(){
	TcpClient::closeConnection();
	WaitForSingleObject(sendThread, INFINITE);
}

void RemoteSoarCommunicator::sendCommandToEv3(Ev3Command command, Identifier* id) {
    WaitForSingleObject(mutex, INFINITE);
    
    // Need to send the command over LCM and wait for the ack
    uint ack = nextAck++;
    command.ack = ack;
    waitingCommands[ack] = command;
    waitingIdentifiers[ack] = id;
    
    ReleaseMutex(mutex);
}

DWORD WINAPI  RemoteSoarCommunicator::sendThreadFunction(void* arg) {
    RemoteSoarCommunicator* soarComm = (RemoteSoarCommunicator*) arg;

    while (soarComm->isConnected()) {
        soarComm->sendCommands();
        Sleep(1000 / SOAR_SEND_COMMAND_FPS);
    }

    return 0;
}

void RemoteSoarCommunicator::sendCommands() {
    WaitForSingleObject(mutex, INFINITE);
    
    // Send Message to Ev3 of all queued commands
    // Add Commands to the message
    if (waitingCommands.size() > 0) {
        IntBuffer buffer;
        buffer.push_back(COMMAND_MESSAGE);
        buffer.push_back(waitingCommands.size());

        for (CommandMapIt i = waitingCommands.begin(); i != waitingCommands.end(); i++) {
            i->second.packCommand(buffer);
        }

        char* outBuffer;
        uint buf_size;
        packBuffer(buffer, outBuffer, buf_size);
        sendPacket(outBuffer, buf_size);
        delete [] outBuffer;
    }

    ReleaseMutex(mutex);
}

void RemoteSoarCommunicator::receiveMessage(const void* buffer, int buf_len, void* user) {
    RemoteSoarCommunicator* comm = (RemoteSoarCommunicator*) user;

    IntBuffer params;
    uint offset = 0;
    unpackBuffer((const char*) buffer, buf_len, params);

    // Handle 1 message at a time
	comm->receiveStatusMessage(params, offset);
}

void RemoteSoarCommunicator::receiveStatusMessage(IntBuffer& buffer, uint& offset) {
    //cout << "--> Soar Receive Status" << endl;
    WaitForSingleObject(mutex, INFINITE);
    
    int send_time = buffer[offset++];
    
    uint numAcks = buffer[offset++];
    for (uint i = 0; i < numAcks; i++) {
        uint ack = buffer[offset++];
        CommandMapIt it = waitingCommands.find(ack);
        if (it != waitingCommands.end()) {
            waitingCommands.erase(it);
        }
        IdentifierMap::iterator jt = waitingIdentifiers.find(ack);
        if (jt != waitingIdentifiers.end()) {
            //cout << "Added "<< ack << " to finished ids " << jt->second->GetValueAsString() << endl;
            finishedIdentifiers.insert(jt->second);
            waitingIdentifiers.erase(jt);
        }
    }

    ReleaseMutex(mutex);
    
    StatusList statuses;
    uint numStatuses = buffer[offset++];
    for (uint i = 0; i < numStatuses; i++) {
        statuses.push_back(Ev3Status(buffer, offset));
    }
	soarManager->readStatus(statuses);
    //cout << "<-- Soar Receive Status" << endl;
}

void RemoteSoarCommunicator::updateSoar() {
    WaitForSingleObject(mutex, INFINITE);
    
    for (IdentifierSet::iterator i = finishedIdentifiers.begin(); i != finishedIdentifiers.end(); i++) {
        (*i)->CreateStringWME("status", "success");
    }
    finishedIdentifiers.clear();

    ReleaseMutex(mutex);
}
