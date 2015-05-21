#include <iostream>
#include <string>

#include <unistd.h>

#include "comm/RemoteSoarCommunicator.h"
#include "soar/SoarManager.h"

using namespace std;


int main(int argc, char** argv){
	if(argc < 3){
		cout << "Param 1: IP addrss of Robot" << endl;
		cout << "Param 2: filepath for agent source file" << endl;
		return 0;
	}

	
	string filepath = argv[2];
	string parentPath = filepath.substr(0, filepath.find_last_of("/\\"));
	string filename = filepath.substr(filepath.find_last_of("/\\")+1, -1);
	
	if (parentPath.size() == 0)
	{
		parentPath = filepath.substr(0, filepath.find_last_of("/"));
		filename = filepath.substr(filepath.find_last_of("/")+1, -1);
	}
	
	chdir(parentPath.c_str());
	
  SoarManager manager(filename.c_str(), true);
  RemoteSoarCommunicator comm(&manager, argv[1]);
  manager.setCommunicator(&comm);

  comm.openConnection();

	while(manager.isRunning()){
    if(!comm.isConnected()){
      printf("Trying to reconnect\n");
      comm.openConnection();
      if(!comm.isConnected()){
        // Failed to open, sleep for 5 seconds before trying
        sleep(5);
      }
    }
    sleep(1);
	}

  comm.closeConnection();

	return 0;
}
