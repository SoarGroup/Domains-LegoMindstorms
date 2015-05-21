#include <iostream>

#include "comm/Ev3Communication.h"
#include "ev3/Ev3Manager.h"

#include <unistd.h>

using namespace std;

int main(){
  Ev3Manager manager;
	RemoteEv3Communicator comm(&manager);
  comm.start();

  while(comm.isActive()){
    sleep(10);
  }

	return 0;
}
