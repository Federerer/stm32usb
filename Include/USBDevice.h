/*
 * USBDevice.h
 *
 *  Created on: 13 sie 2017
 *      Author: 08fed
 */

#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include <cstdint>
#include "USBDeviceConfigration.h"
#include "USBLowLevel.h"
#include "SetupPacket.h"
#include "ControlTransfer.h"

#include <initializer_list>

namespace USBLib
{

class USBDevice
{
public:
	constexpr USBDevice(const std::initializer_list<const USBDeviceConfigration*> config, USBLowLevel& lowLevelDriver) :
			_config
			{ *config.begin() }, _lowLevelDriver
			{ lowLevelDriver },
			_transfer{}
	{
		_lowLevelDriver.AttachDeviceDriver(this);
	}

	virtual void connect();
	virtual void disconnect();

	virtual void busReset(void);
	virtual void EP0setupCallback(void);
	virtual void EP0out(void);
	virtual void EP0in(void);
	virtual void connectStateChanged(bool connected) = 0;
	virtual void suspendStateChanged(bool suspended) = 0;
	virtual void SOF(int frameNumber) = 0;

	virtual ~USBDevice();
//protected:
	const uint8_t* const getDeviceDescriptor();

private:
	bool handleStandardRequest();
	bool handleGetDescriptor();
	bool handleSetAddress();
	bool handleControlIn();
	bool handleControlOut();
	bool handleControlDataInStage();

	const USBDeviceConfigration* _config;
	USBLowLevel& _lowLevelDriver;

	ControlTransfer _transfer;

	uint8_t _address
	{ 0 };
};

} /* namespace USB */

#endif /* USBDEVICE_H_ */
