/*
 * CommStructs.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 *  Contains structures used during communication
 *    And ways to pack/unpack them from buffers
 *
 *  CommandHandler - An interface for a device that can handle Ev3Commands
 *  Ev3Status - Contains status information about a single device
 *  Ev3Command - Contains a single command for the robot
 */

#ifndef COMMSTRUCTS_H_
#define COMMSTRUCTS_H_


#include "Constants.h"

#include "util/CommUtil.h"

// Vectors
#include <vector>

struct Ev3Status;
typedef std::vector<Ev3Status> StatusList;

// Sets
#include <set>

typedef std::set<uint> AckSet;
typedef AckSet::iterator AckSetIt;

// Maps
#include <map>

struct Ev3Command;
typedef std::map<uint, Ev3Command> CommandMap;
typedef CommandMap::iterator CommandMapIt;

// CommandHandler
//   An interface for a device that can handle Ev3Commands
class CommandHandler{
public:
	virtual ~CommandHandler(){};
	virtual void handleCommand(Ev3Command* command) = 0;
};

// Ev3Command
//   Wraps up a single command for a specific device
struct Ev3Command{
public:
	Ev3Command(): ack(0), dev(0){}

	Ev3Command(uint ack, ushort dev, const IntBuffer& params)
		:ack(ack), dev(dev), params(params){}

  // Pushes the command onto the given buffer
	void packCommand(IntBuffer& buffer);

  // Reads the command from the buffer starting at the given offset
  // When the command is finished, offset is set to the first index after this commands representation
	void unpackCommand(const IntBuffer& buffer, uint& offset);

  // Sort operator
	bool operator < (const Ev3Command& cmd2){
		return ack < cmd2.ack;
	}

	// Variables
	uint ack;  // Unique number used for acknowledgements
	ushort dev; // id for the device this command is being sent to
	IntBuffer params; // Additional parameters that define the command
};

// Ev3Status
//   Wraps up a status update about a particular device
struct Ev3Status{
public:
	Ev3Status(const IntBuffer& buffer, uint& offset){
		unpackStatus(buffer, offset);
	}

	Ev3Status(ushort dev):dev(dev){}

  // Pushes the status information onto the given buffer
	void packStatus(IntBuffer& buffer);

  // Reads the status from the buffer starting at the given offset
  // When the method is done, offset will be set to the buffer index after this status
	void unpackStatus(const IntBuffer& buffer, uint& offset);

  // sort operator
	bool operator < (const Ev3Status& s2){
		return dev < s2.dev;
	}

	// Variables
	ushort dev;   // Id of the device this status is about
	IntBuffer info; // Status information about the device
};

#endif /* COMMSTRUCTS_H_ */
