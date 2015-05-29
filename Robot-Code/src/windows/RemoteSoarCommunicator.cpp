/*
 * SoarCommunication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "windows/RemoteSoarCommunicator.h"

#include "Constants.h"
#include "comm/CommStructs.h"

#include "soar/SoarManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;
using namespace sml;


// RemoteSoarCommunicator

RemoteSoarCommunicator::RemoteSoarCommunicator(SoarManager* manager, string server_ip)
	: SoarCommunicator(manager), TcpClient(server_ip), nextAck(1){
    mutex = CreateMutex(NULL, FALSE, NULL);
    if(mutex == NULL){
        printf("CreateMutex error: %d\n", GetLastError());
    }
    this->setReceptionCallback(&RemoteSoarCommunicator::receiveMessage, this);
	
	DWORD threadId;
	sendThread = CreateThread(NULL, 0, &sendThreadFunction, (void*)this, 0, &threadId);
}

RemoteSoarCommunicator::~RemoteSoarCommunicator() {
    CloseHandle(mutex);
	closeConnection();
}

void RemoteSoarCommunicator::reinit(){
	WaitForSingleObject(mutex, INFINITE);

	waitingCommands.clear();
	waitingIdentifiers.clear();
	finishedIdentifiers.clear();

	ReleaseMutex(mutex);
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

    while (soarComm->soarManager->isRunning()) {
        soarComm->sendCommands();
        Sleep(1000 / SOAR_SEND_COMMAND_FPS);
    }

    return 0;
}

void RemoteSoarCommunicator::sendCommands() {
	if (!isConnected()){
		return;
	}

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

    
    StatusList statuses;
    uint numStatuses = buffer[offset++];
    for (uint i = 0; i < numStatuses; i++) {
        statuses.push_back(Ev3Status(buffer, offset));
    }
	soarManager->readStatus(statuses);
    //cout << "<-- Soar Receive Status" << endl;

	ReleaseMutex(mutex);
}

void RemoteSoarCommunicator::inputPhaseCallback() {
    WaitForSingleObject(mutex, INFINITE);
    
    for (IdentifierSet::iterator i = finishedIdentifiers.begin(); i != finishedIdentifiers.end(); i++) {
        (*i)->CreateStringWME("status", "complete");
    }
    finishedIdentifiers.clear();

    ReleaseMutex(mutex);
}
