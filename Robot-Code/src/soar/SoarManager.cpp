/*
 * SoarManager.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#include "comm/SoarCommunicator.h"

#include "soar/SoarManager.h"

#include "soar/Brick.h"
#include "soar/Motor.h"
#include "soar/ColorSensor.h"
#include "soar/TouchSensor.h"
#include "soar/IRRemote.h"

#include "util/CommUtil.h"

#include <fcntl.h>
#include <stdio.h>
#include <fstream>
//
#include <sys/time.h>

using namespace std;
using namespace sml;

SoarManager::SoarManager(std::string agentSource, bool debugger, ostream* logger)
	:comm(0), timeStep(1), running(true), print_stream(logger), got_update(true)
{
	kernel = Kernel::CreateKernelInNewThread();
	agent = kernel->CreateAgent("Ev3 Agent");

	if(debugger){
		agent->SpawnDebugger(kernel->GetListenerPort());
	}

	agent->LoadProductions(agentSource.c_str(), true);

	inputPhaseCallbackId = agent->RegisterForRunEvent(smlEVENT_AFTER_OUTPUT_PHASE, &runEventHandler, (void*)this, true);
  reinitCallbackId = kernel->RegisterForAgentEvent(smlEVENT_BEFORE_AGENT_REINITIALIZED, &agentEventHandler, (void*)this, true);
  if(print_stream != 0){
    cout << "Registering for print event" << endl;
    printCallbackId = agent->RegisterForPrintEvent(sml::smlEVENT_PRINT, &printEventHandler, (void*)this);
  }

	// Manager
	outputCallbackIds.push_back(agent->AddOutputHandler("manager", SoarManager::outputEventHandler, (void*)this));

	// Brick
	brick = new Brick(this);
	outputCallbackIds.push_back(agent->AddOutputHandler("brick", SoarManager::outputEventHandler, (void*)this));

	// Motors
	for(int i = 0; i < NUM_OUTPUTS; i++){
		motors[i] = new Motor(i, this);
	}
	outputCallbackIds.push_back(agent->AddOutputHandler("motor", SoarManager::outputEventHandler, (void*)this));

	// Sensors
	for(int i = 0; i < NUM_INPUTS; i++){
		inputs[i] = 0;
	}
	outputCallbackIds.push_back(agent->AddOutputHandler("sensor", SoarManager::outputEventHandler, (void*)this));
}


SoarManager::~SoarManager(){
	delete brick;
	for(int i = 0; i < NUM_OUTPUTS; i++){
		if(motors[i]){
			delete motors[i];
		}
	}
	for(int i = 0; i < NUM_INPUTS; i++){
		if(inputs[i]){
			delete inputs[i];
		}
	}

	if (agent != 0){
		agent->KillDebugger();
		kernel->Shutdown();
		delete kernel;
	}
}

void SoarManager::step(){
	agent->RunSelf(1);
}

void SoarManager::reinit(){
  comm->reinit();
  brick->reinit();
	for(int i = 0; i < NUM_OUTPUTS; i++){
		if(motors[i]){
      motors[i]->reinit();
		}
	}
	for(int i = 0; i < NUM_INPUTS; i++){
		if(inputs[i]){
      inputs[i]->reinit();
		}
	}
}

void SoarManager::shutdown(){
  if(!running){
    return;
  }

  running = false;
  agent->StopSelf();

  // This will write out stats at the end if uncommented
  ofstream fout;
  fout.open("/media/card/stats");
  fout << agent->ExecuteCommandLine("stats");
  fout.close();

  if(print_stream != 0){
    agent->UnregisterForPrintEvent(printCallbackId);
  }

  agent->UnregisterForRunEvent(inputPhaseCallbackId);
  kernel->UnregisterForAgentEvent(reinitCallbackId);
  for(vector<int>::iterator i = outputCallbackIds.begin(); i != outputCallbackIds.end(); i++){
    agent->RemoveOutputHandler(*i);
  }
  outputCallbackIds.clear();
}

void SoarManager::sendCommandToEv3(Ev3Command command, sml::Identifier* id){
	if (running && comm != 0){
		comm->sendCommandToEv3(command, id);
	}
}

// input phase callback
void SoarManager::agentEventHandler(sml::smlAgentEventId eventID, void* data, Agent* agent){
	SoarManager* manager = (SoarManager*)data;
  if(!manager->isRunning()){
    return;
  }
  if(eventID == smlEVENT_BEFORE_AGENT_REINITIALIZED){
    manager->reinit();
	}
	//Sleep(SOAR_DC_WAIT);
}

// input phase callback
void SoarManager::runEventHandler(sml::smlRunEventId eventID, void* data, Agent* agent, sml::smlPhase phase){
	SoarManager* manager = (SoarManager*)data;
  if(!manager->isRunning()){
    return;
  }
  if(eventID == smlEVENT_AFTER_OUTPUT_PHASE){
		manager->updateInputLink(agent->GetInputLink());
    manager->comm->inputPhaseCallback();
	}
	//Sleep(SOAR_DC_WAIT);
}


void SoarManager::printEventHandler(sml::smlPrintEventId eventID, void* data, sml::Agent* agent, const char* message){
	SoarManager* manager = (SoarManager*)data;
	if (manager->print_stream != 0){
		(*manager->print_stream) << message;
	}
}

// output link event callback
void SoarManager::outputEventHandler(void* data, Agent* agent, const char* attName, WMElement* wme){
	SoarManager* manager = (SoarManager*)data;
  if(manager->isRunning()){
	  manager->handleOutput(attName, wme);
  }
}

void SoarManager::readStatus(StatusList& statuses){
  got_update = true;
	for(unsigned int i = 0; i < statuses.size(); i++){
		Ev3Status& status = statuses[i];
		uint offset = 0;
		//cout << "STATUS FOR: " << status.dev << endl;
		if(status.dev == BRICK_DEV){
			brick->readStatus(status.info, offset);
		} else if(status.dev == OUTPUT_MAN_DEV){
			readMotorsStatus(status.info);
		} else if(status.dev == INPUT_MAN_DEV){
			readSensorsStatus(status.info);
		} else if(status.dev == MANAGER_DEV){

		} else {
			cout << "UNKNOWN DEVICE " << status.dev << endl;
		}
	}
}

void SoarManager::readMotorsStatus(IntBuffer& status){
	uint offset = 0;
	for(int port = 0; port < NUM_OUTPUTS; port++){
		motors[port]->readStatus(status, offset);
	}
}

void SoarManager::readSensorsStatus(IntBuffer& status){
	uint offset = 0;
	uint numSensors = status[offset++];

	for(uint i = 0; i < numSensors; i++){
		uint sensorInfo = status[offset];
		uchar cat, port, type, mode;
		unpackBytes(sensorInfo, cat, port, type, mode);

		switch(cat){
		case SENSOR_CAT_NONE:
			if(inputs[port] != 0){
				delete inputs[port];
				inputs[port] = 0;
			}
			offset += 1;
			break;
		case SENSOR_CAT_SERIAL:
			if(inputs[port] != 0 && type == 125){
				uint offsetCopy = offset;
				inputs[port]->readStatus(status, offsetCopy);
				offset += 3;
				break;
			}
		case SENSOR_CAT_ANALOG:
			if(inputs[port] != 0 && inputs[port]->getDeviceType() != type){
				delete inputs[port];
				inputs[port] = 0;
			}
			if(inputs[port] == 0){
				createSensor(port, type);
			}
			if(inputs[port] != 0){
				uint offsetCopy = offset;
				inputs[port]->readStatus(status, offsetCopy);
			}
			if(cat == SENSOR_CAT_SERIAL){
				offset += 3;
			} else {
				offset += 2;
			}
			break;
		default:
			offset += 1;
		}
	}
}

void SoarManager::createSensor(uint port, uint type){
	switch(type){
	case EV3_COLOR_SENSOR_TYPE:
		inputs[port] = new ColorSensor(port+1, this);
		break;
	case EV3_IR_REMOTE_SENSOR_TYPE:
		inputs[port] = new IRRemote(port + 1, this);
		break;
	case EV3_TOUCH_SENSOR_TYPE:
		inputs[port] = new TouchSensor(port + 1, this);
		break;
	}
}

void SoarManager::updateInputLink(Identifier* inputLink){
	timeStep++;
	WMUtil::updateIntWME(inputLink, "time-step", timeStep);

  // Only update if we actually got new info
  if(!got_update){
    return;
  }
  got_update = false;

	brick->updateInputLink(inputLink);
	for(int i = 0; i < 4; i++){
		if(inputs[i]){
			inputs[i]->updateInputLink(inputLink);
		}
		if(motors[i]){
			motors[i]->updateInputLink(inputLink);
		}
	}
}

void SoarManager::handleOutput(string attName, WMElement* wme){
	if(!wme->IsJustAdded() || !wme->IsIdentifier()){
		return;
	}

	SoarDevice* dev = 0;
	Identifier* id = wme->ConvertToIdentifier();
	string att = string(attName);
	//cout << "ATT: " << att << endl;
	if(att == "brick"){
		dev = brick;
	} else if(att == "motor"){
		string port;
		if(WMUtil::getValue(id, "port", port)){
			int portNum = port[0] - 'A';
			if(portNum >= 0 && portNum <= 4){
				dev = motors[portNum];
			}
		}
	} else if(att == "sensor"){
		int port;
		if(WMUtil::getValue(id, "port", port)){
			if(port >= 1 && port <= 4){
				dev = inputs[port-1];
			}
		}
	} else if(att == "manager"){
		if(!this->readSoarCommand(id)){
			id->CreateStringWME("status", "error");
		}
	}	else {
		id->CreateStringWME("error", "Unrecognized command");
		id->CreateStringWME("status", "error");
		return;
	}
	if(dev){
		if(!dev->readSoarCommand(id)){
			id->CreateStringWME("status", "error");
		}
	}
}

bool SoarManager::readSoarCommand(Identifier* id){
	Identifier* childId;

  std::string childStr;
  if (WMUtil::getValue(id, "exit", childStr)){
    shutdown();
  }

	if(WMUtil::getValue(id, "create-sensor", childId)){
		if(!handleCreateSensorCommand(childId)){
			return false;
		}
	}

	if(WMUtil::getValue(id, "delete-sensor", childId)){
		if(!handleDeleteSensorCommand(childId)){
			return false;
		}
	}
	return true;
}

bool SoarManager::handleCreateSensorCommand(Identifier* id){
	Ev3Command command;
	command.dev = INPUT_MAN_DEV;

	string type;
	if(!WMUtil::getValue(id, "type", type)){
		return false;
	}

	int port;
	if(!WMUtil::getValue(id, "port", port)){
		return false;
	}

	if(type == "touch"){
		command.params.push_back(CREATE_ANALOG_SENSOR_COMMAND);
		command.params.push_back(packBytes(port-1, EV3_TOUCH_SENSOR_TYPE, 0, 0));
	}

	this->sendCommandToEv3(command, id);
	return true;
}

bool SoarManager::handleDeleteSensorCommand(Identifier* id){
	Ev3Command command;
	command.dev = INPUT_MAN_DEV;

	int port;
	if(!WMUtil::getValue(id, "port", port)){
		return false;
	}

	command.params.push_back(DELETE_ANALOG_SENSOR_COMMAND);
	command.params.push_back(packBytes(port-1, 0, 0, 0));

	this->sendCommandToEv3(command, id);
	return true;
}

