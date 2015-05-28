/*
 * SoarDevice.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 *
 *  Abstract base class for all devices that interface with soar
 */

#ifndef SOARDEVICE_H_
#define SOARDEVICE_H_

#include "util/CommUtil.h"
#include "sml_Client.h"

class SoarDevice{
public:
	virtual ~SoarDevice(){}

  // Called when there is an init-soar
  //   The device should remove any references to sml objects
  virtual void reinit() = 0;

  // Called before every input phase
  //   The device should update the input link here
	virtual void updateInputLink(sml::Identifier* inputLink) = 0;

  // Interpret a soar command rooted at the given identifier
  //   Return true if the command was successfully interpreted
	virtual bool readSoarCommand(sml::Identifier* command) = 0;
  
  // Read status information for the device starting at the given offset
  //   After this is called, offset should point to the next index
  //   after the devices status information 
	virtual void readStatus(IntBuffer& buffer, uint& offset) = 0;

  // Return the type associated with this device
  // (See Constants.h)
	virtual uint getDeviceType() = 0;
};

#endif /* SOARDEVICE_H_ */
