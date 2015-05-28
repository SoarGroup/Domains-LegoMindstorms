/*
 * SoarManager.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 *
 *  SoarManager
 *    This class is the interface to a running soar instance
 *    It creates the kernel/agent and listens to events
 *    It schedules updates to the input-link and gets commands on the output-link
 *    It also creates and manages the SoarDevices
 */

#ifndef SOARMANAGER_H_
#define SOARMANAGER_H_

#include "comm/CommStructs.h"

class SoarCommunicator;
class SoarDevice;
class Brick;
class Motor;

#include "sml_Client.h"

#include <ostream>

#include <string>
#include <map>
typedef std::map<uint, Motor*> OutputDevices;
typedef std::map<uint, SoarDevice*> InputDevices;

class SoarManager {
public:
	SoarManager(std::string agentSource, bool debugger);
	virtual ~SoarManager();
	
  // Assigns the communicator used by the Manager to communicate with the Ev3Manager
  // THIS MUST BE ASSIGNED
	void setCommunicator(SoarCommunicator* comm){
		this->comm = comm;
	}
	
  // Given a set of status updates for devices, 
  //   Sends each status to the correct device
	void readStatus(StatusList& statuses);
	
  // Steps the agent a single decision cycle
	void step();
	
  // Called when there is an init-soar event
	void reinit();
	
  // Returns true if the soar agent is alive and being used
	bool isRunning(){
		return running;
	}
	
  // Stops the agent, deregisters event handlers, and destroys the kernel
	void shutdown();
	
	// smlRunEvent callback (called before each input phase)
	static void runEventHandler(sml::smlRunEventId eventID, void* data, sml::Agent* agent, sml::smlPhase phase);
	
  // smlAgentEvent callback (used for init-soar event)
	static void agentEventHandler(sml::smlAgentEventId eventID, void* data, sml::Agent* agent);

  // smlPrintEvent callback (used for print events to write a log)
	static void printEventHandler(sml::smlPrintEventId eventID, void* data, sml::Agent* agent, const char* message);

	// output link event callback
	static void outputEventHandler(void* data, sml::Agent* agent, const char* attName,  sml::WMElement* wme);

  // sets the output stream used to log print events
	void setPrintStream(ostream* stream){
		print_stream = stream;
	}
	
  // Called when a new structure is created on the output link
	void handleOutput(std::string attName, sml::WMElement* wme);
	
  // Send a command to the Ev3Manager
	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);
	
private:
  // Read status updates for sensors
	void readSensorsStatus(IntBuffer& status);

  // Read status updates for the motors
	void readMotorsStatus(IntBuffer& status);

  // Creates a new sensor device
	void createSensor(uint port, uint type);

  // Updates the input link and calles updateInputLink on all devices
	void updateInputLink(sml::Identifier* inputLink);

  // Handles soar commands intended for the SoarManager
	bool readSoarCommand(sml::Identifier* id);

  // Handles a soar command to create a sensor (used for touch sensor)
	bool handleCreateSensorCommand(sml::Identifier* id);
  
  // Handles a soar command to delete a sensor
	bool handleDeleteSensorCommand(sml::Identifier* id);
	
	SoarCommunicator* comm;
	
	Brick* brick;
	Motor* motors[4];
	SoarDevice* inputs[4];
	
	// Soar agent variables
	sml::Kernel* kernel;
	sml::Agent* agent;
	int timeStep;
	
	bool running;
	
  // Callback ids for soar events
  //  (used to deregister)
	int reinitCallbackId;
	int inputPhaseCallbackId;
	int printCallbackId;
	std::vector<int> outputCallbackIds;

	ostream* print_stream;
};

#endif /* SOARMANAGER_H_ */

