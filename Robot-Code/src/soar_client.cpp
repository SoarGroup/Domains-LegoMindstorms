
#ifdef _WIN32
	// Includes for windows
	#include "windows/RemoteSoarCommunicator.h"
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	// Includes for Linux/Mac
	#include "comm/RemoteSoarCommunicator.h"
	#include <unistd.h>
#endif

#include "soar/SoarManager.h"

#include <iostream>
#include <string>

using namespace std;

void sleep_secs(int num_secs){
#ifdef _WIN32
	Sleep(1000 * num_secs);
#else
	sleep(5);
#endif
}

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
	
	// Change the directory
#ifdef _WIN32
	wstring parentPathW;
	parentPathW.assign(parentPath.begin(), parentPath.end());
	SetCurrentDirectory(parentPathW.c_str());
#else
	chdir(parentPath.c_str());
#endif
	
  SoarManager manager(filename.c_str(), true);
  RemoteSoarCommunicator comm(&manager, argv[1]);
  manager.setCommunicator(&comm);
  manager.setPrintStream(&cout);

  comm.openConnection();

  while(manager.isRunning()){
    if(!comm.isConnected()){
      printf("Trying to reconnect\n");
      comm.openConnection();
      if(!comm.isConnected()){
        // Failed to open, sleep for 5 seconds before trying
        sleep_secs(5);
      }
    }
	sleep_secs(1);
  }

  comm.closeConnection();

  return 0;
}
