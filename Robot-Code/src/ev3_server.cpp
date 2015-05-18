#include <iostream>

#include "comm/Ev3Communication.h"
#include "ev3/Ev3Manager.h"

#include <unistd.h>

using namespace std;

int main(){
	RemoteEv3Communicator comm;
	Ev3Manager manager;
	comm.assignManager(&manager);
	comm.start();

	while(true){
		sleep(10);
	}

	return 0;
}
