#include <iostream>
#include <fstream>
#include <string>

#include "comm/DirectCommunicator.h"
#include "ev3/Ev3Manager.h"
#include "soar/SoarManager.h"

using namespace std;

int main(int argc, char** argv){
  string agent_source = "";
  string logging = "none";

  ifstream fin;
  fin.open("/media/card/config");
  if(!fin.fail()){
    string arg_name;
    while(fin >> arg_name){
      if (arg_name == "source"){
        fin >> agent_source;
      } else if (arg_name == "logging"){
        fin >> logging;
      }
    }
    fin.close();
  }

  if(argc > 1){
    agent_source = argv[1];
  }
  if(argc > 2){
    logging = argv[2];
  }

  ofstream fout;
  ostream* logger = 0;
  if(logging == "file"){
    fout.open("/media/card/log");
    if(fout.fail()){
      logging = "none";
    } else {
      logger = &fout;
    }
  } else if (logging == "cout"){
    logger = &cout;
  }

	Ev3Manager em;
  SoarManager sm(agent_source, false, logger);
	DirectCommunicator comm(&sm, &em);
  sm.setCommunicator(&comm);

	while(sm.isRunning()){
		sm.step();
	}

  comm.shutdown();

  if(logging == "file"){
    fout.close();
  }

	return 0;
}
