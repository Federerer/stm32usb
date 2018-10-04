/*
 * IUSBLowLevel.h
 *
 *  Created on: 13 sie 2017
 *      Author: 08fed
 */

#ifndef IUSBLOWLEVEL_H_
#define IUSBLOWLEVEL_H_

#include <cstdint>

namespace USBLib
{

class USBDevice;

class USBLowLevel
{
public:
	USBLowLevel();
	virtual bool connect() = 0;
	virtual bool disconnect() = 0;
	virtual bool setAddress(uint8_t address) = 0;
	virtual bool readSetupHeader(uint8_t* buffer) = 0;
	virtual bool writeEndpoint(uint8_t number, const uint8_t* data, uint16_t len) = 0;
	virtual uint16_t readEndpointData(uint8_t number, uint8_t* buffer) = 0;
	virtual void readEndpoint(uint8_t number) = 0;
	inline bool AttachDeviceDriver(USBDevice* device)
	{
		_deviceDriver = device;
	}
	virtual ~USBLowLevel();
protected:
	USBDevice* _deviceDriver;
public:
	virtual void _usbISR(void);
};

} /* namespace USB */

inline USBLib::USBLowLevel::USBLowLevel()
{
}

inline USBLib::USBLowLevel::~USBLowLevel()
{
}

inline void USBLib::USBLowLevel::_usbISR(void)
{
}

#endif /* IUSBLOWLEVEL_H_ */
