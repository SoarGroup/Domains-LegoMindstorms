#include <iostream>
#include <fstream>
#include <string>

#include "comm/DirectCommunication.h"
#include "ev3/Ev3Manager.h"
#include "soar/SoarManager.h"

using namespace std;

int main(int argc, char** argv){
  ofstream fout;
  fout.open("/media/card/log");

  ifstream fin;
  fin.open("/media/card/config");

  int channel = 1;
  string agent_source = "";

  string arg_name;
  while(fin >> arg_name){
    if (arg_name == "source"){
      fin >> agent_source;
      fout << "Agent Source = " << agent_source << endl;
    }
  }

	Ev3Manager em;
  SoarManager sm(agent_source, false);
	DirectCommunicator comm(&sm, &em);
	comm.start();

	while(sm.isRunning()){
		sm.step();
	}

  fin.close();
  fout.close();

	return 0;
}
