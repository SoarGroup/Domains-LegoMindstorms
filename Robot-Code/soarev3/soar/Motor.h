#ifndef __SOAR_MOTOR_H__
#define __SOAR_MOTOR_H__

#include "SoarDevice.h"

#include "Constants.h"

class SoarManager;

class Motor : public SoarDevice {
public:
	Motor(uchar port, SoarManager* manager);

  void reinit();

	void readStatus(IntBuffer& buffer, uint& offset);

	void updateInputLink(sml::Identifier* parentId);

	bool readSoarCommand(sml::Identifier* commandId);

	uint getDeviceType(){
		return LARGE_MOTOR_TYPE;
	}

private:
	uchar port;
	string portStr;

	SoarManager* manager;

	int speed;
	int tachoCount;
	int tachoSensor;

	sml::Identifier* motorId;
};

#endif
