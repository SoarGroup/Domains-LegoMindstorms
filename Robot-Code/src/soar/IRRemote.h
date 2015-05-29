/*
 * IRRemote.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 * RemoteButton
 *  Represents a single button on the ir remote
 *
 * IRRemote : SoarDevice
 *  Soar interface to the ir remote
 */

#ifndef IRREMOTE_SENSOR_H_
#define IRREMOTE_SENSOR_H_

#include "SoarDevice.h"

#include "Constants.h"

#include <string>

class SoarManager;

/*******************************************
 *
 * RemoteButton
 *
 ********************************************/

class RemoteButton{
public:
	RemoteButton(ushort buttonType);

	~RemoteButton();

  void reinit();

	void setState(int stateCode);

	void updateInputLink(sml::Identifier* parentId);

	uint getDeviceType(){
		return 0;
	}

private:
	std::string name;
	ushort buttonType;
	sml::Identifier* id;

	bool curState;
	bool prevState;
};

/*******************************************
 *
 * IRRemote Sensor
 *
 ********************************************/

class IRRemote : public SoarDevice{
public:
	IRRemote(uint port, SoarManager* manager);
	~IRRemote();

  void reinit();

	void updateInputLink(sml::Identifier* inputLink);
	void changeSoarMode(uchar newMode);

	void readStatus(IntBuffer& buffer, uint& offset);

	bool readSoarCommand(sml::Identifier* commandId);

	uint getDeviceType(){
		return EV3_IR_REMOTE_SENSOR_TYPE;
	}

private:
	SoarManager* manager;

	uint port;
	uchar mode;
	uchar soarMode;

	uint channel;
	char distance; // 0 (close) to 100 (far), -128 means no signal
	char direction; // -25 (left) to 25 (right), 0 is straight ahead

	uchar buttonState;
	RemoteButton* buttons[NUM_IR_REMOTE_BUTTONS];

	sml::Identifier* rootId;
};

#endif /* IRREMOTE_SENSOR_H_ */
