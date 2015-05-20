/*
 * SoarCommunication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARCOMMUNICATION_H_
#define SOARCOMMUNICATION_H_

#include "windows/TcpClient.h"

#include <string>

class SoarManager;

#include "sml_Client.h"

#include "comm/CommStructs.h"


#include <ctime>

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class SoarCommunicator {
public:
	SoarCommunicator(SoarManager* manager);

    virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

    virtual void updateSoar() = 0;

	virtual bool openConnection(){
		return true;
	}

	virtual void closeConnection(){}

	virtual bool isConnected() = 0;
protected:
	SoarManager* soarManager;
};

class RemoteSoarCommunicator : public SoarCommunicator, public TcpClient {
public:
    RemoteSoarCommunicator(SoarManager* manager, string server_ip);

    virtual ~RemoteSoarCommunicator();

    virtual bool openConnection();

	virtual void closeConnection();

    virtual bool isConnected() {
        return TcpClient::isConnected();
    }

    void assignManager(SoarManager* manager) {
        soarManager = manager;
    }

    void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

    void updateSoar();

private:
    static DWORD WINAPI sendThreadFunction(void* arg);

    void sendCommands();

    static void receiveMessage(const void* buffer, int buf_len, void* user);

    void receiveAckMessage(IntBuffer& buffer, uint& offset);

    void receiveStatusMessage(IntBuffer& buffer, uint& offset);

private:
    HANDLE sendThread;
    HANDLE mutex;

    uint nextAck;
    CommandMap waitingCommands;
    IdentifierMap waitingIdentifiers;
    IdentifierSet finishedIdentifiers;
};


#endif /* SOARCOMMUNICATION_H_ */
