/*
 * SoarBrick.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 *  BrickButton 
 *    Wrapper for a single button on the brick
 *
 *  SoarBrick : SoarDevice
 *    Implements the soar interface to the lego brick
 *    Includes the buttons and leds
 */

#ifndef SOARBRICK_H_
#define SOARBRICK_H_

#include "SoarDevice.h"

class SoarManager;

#include "sml_Client.h"
#include "util/WMUtil.h"
using namespace sml;

class BrickButton{
public:
  // The mask for the specific byte in the buttonstate
  //   correponding to this button
	BrickButton(std::string name, int mask)
	:name(name), mask(mask), id(0), curState(false), prevState(false){

	}

	~BrickButton(){
		if(id){
			id->DestroyWME();
			id = 0;
		}
	}

  void reinit();

	void setState(int buttonState){
		curState = ((buttonState & mask) > 0);
	}

	void updateInputLink(Identifier* parentId);

private:
	std::string name;
	int mask;

	Identifier* id;

	bool curState;
	bool prevState;
};

/*****************************************
 *
 * Brick
 *
 *****************************************/

#include <vector>
#include <map>
#include <string>

typedef std::vector<BrickButton*> ButtonVector;
typedef ButtonVector::iterator ButtonVectorIt;

typedef std::map<std::string, int> LedMap;
typedef LedMap::iterator LedMapIt;

class Brick : public SoarDevice{
public:
	Brick(SoarManager* manager);
	~Brick();

  void reinit();
	void updateInputLink(Identifier* inputLink);
	bool readSoarCommand(Identifier* command);
	void readStatus(IntBuffer& buffer, uint& offset);
	uint getDeviceType(){
		return 0;
	}

private:
	SoarManager* manager;

	Identifier* brickId;

	ButtonVector buttons;
	LedMap leds;
};


#endif /* SOARBRICK_H_ */
