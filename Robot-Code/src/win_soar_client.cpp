/*
* File:   main.cpp
* Author: Aaron
*
* Created on May 19, 2015, 11:09 AM
*/


#include "windows/SoarCommunication.h"
#include "soar/SoarManager.h"

#include <iostream>
#include <string>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Param 1: IP addrss of Robot" << endl;
		cout << "Param 2: filepath for agent source file" << endl;
		return 0;
	}


	string filepath(argv[2]);
	string parentPath = filepath.substr(0, filepath.find_last_of("/\\"));
	string filename = filepath.substr(filepath.find_last_of("/\\") + 1, -1);

	if (parentPath.size() == 0) {
		parentPath = filepath.substr(0, filepath.find_last_of("/"));
		filename = filepath.substr(filepath.find_last_of("/") + 1, -1);
	}

	wstring parentPathW;
	parentPathW.assign(parentPath.begin(), parentPath.end());
	SetCurrentDirectory(parentPathW.c_str());

	SoarManager* manager = new SoarManager(filename.c_str(), true);
	RemoteSoarCommunicator* comm = new RemoteSoarCommunicator(manager, argv[1]);
	manager->setCommunicator(comm);
	comm->openConnection();

	while (manager->isRunning()){
		if (!comm->isConnected()) {
			comm->openConnection();
			if (!comm->isConnected()){
				printf("Trying to reconnect to server\n");
				// Try to restart if there was a bad connection after 5 seconds
				Sleep(5000);
			}
		}
		Sleep(100);
	}

	delete manager;

	return 0;
}

