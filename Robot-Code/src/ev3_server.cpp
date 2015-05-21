#include <iostream>

#include "comm/RemoteEv3Communicator.h"
#include "ev3/Ev3Manager.h"

#include <unistd.h>

using namespace std;

int main(){
  Ev3Manager manager;
	RemoteEv3Communicator comm(&manager);
  comm.start();

  while(true){
    // Just let the server go on indefinitely
    sleep(10);
  }

	return 0;
}
