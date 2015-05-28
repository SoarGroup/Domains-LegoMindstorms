#include <iostream>
#include <fstream>
#include <string>

#include "comm/DirectCommunicator.h"
#include "ev3/Ev3Manager.h"
#include "soar/SoarManager.h"

using namespace std;

int main(int argc, char** argv){
  ifstream fin;
  fin.open("/media/card/config");

  int channel = 1;
  string agent_source = "";
  bool log = false;

  string arg_name;
  string arg_val;
  while(fin >> arg_name){
    if (arg_name == "source"){
      fin >> agent_source;
    } else if (arg_name == "logging" || arg_name == "log"){
      fin >> arg_val;
      log = (arg_val == "true");
    }
  }

  fin.close();

	Ev3Manager em;
  SoarManager sm(agent_source, false);
	DirectCommunicator comm(&sm, &em);
  sm.setCommunicator(&comm);

  ofstream fout;
  if(log){
    fout.open("/media/card/log");
    sm.setPrintStream(&fout);
  }

	while(sm.isRunning()){
		sm.step();
	}

  comm.shutdown();

  if(log){
    fout.close();
  }

	return 0;
}
