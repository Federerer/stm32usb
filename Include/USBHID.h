/*
 * USBHID.h
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#ifndef USBHID_H_
#define USBHID_H_

#include "USBDevice.h"
#include "USBDeviceConfigration.h"

namespace USBLib
{

class USBHID : public USBDevice
{
public:
	constexpr USBHID(const USBDeviceConfigration* config, USBLowLevel& lowLevelDriver) : USBDevice({config}, lowLevelDriver)
	{
		// TODO Auto-generated constructor stub
	}

	virtual void connectStateChanged(bool connected) override;
	virtual void suspendStateChanged(bool suspended) override;
	virtual void SOF(int frameNumber) override;
	virtual ~USBHID();
};

} /* namespace USB */

#endif /* USBHID_H_ */
