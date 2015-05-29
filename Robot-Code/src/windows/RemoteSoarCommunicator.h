/*
 * SoarCommunication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef _REMOTE_SOAR_COMMUNICATOR_H_
#define _REMOTE_SOAR_COMMUNICATOR_H_

#include "comm/SoarCommunicator.h"
#include "windows/TcpClient.h"

#include <string>
#include <set>
#include <map>

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class RemoteSoarCommunicator : public SoarCommunicator, public TcpClient {
public:
    RemoteSoarCommunicator(SoarManager* manager, string server_ip);

    virtual ~RemoteSoarCommunicator();

	void reinit();

    void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

    void inputPhaseCallback();

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


#endif /* _REMOTE_SOAR_COMMUNICATOR_H_ */
