/*
 * USBLowLevel_STM32F0.h
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#ifndef TARGET_STM32F0_USBLOWLEVEL_STM32F0_H_
#define TARGET_STM32F0_USBLOWLEVEL_STM32F0_H_

#include "USBLowLevel.h"
#include <cstdint>

namespace USBLib
{

class USBLowLevel_STM32F0: public USBLowLevel
{
public:
	USBLowLevel_STM32F0();
	bool connect() override;
    bool disconnect() override;
    virtual bool setAddress(uint8_t address) override;
    bool readSetupHeader(uint8_t* buffer) override;
    virtual bool writeEndpoint(uint8_t number, const uint8_t* data, uint16_t len) override;
    virtual uint16_t readEndpointData(uint8_t number, uint8_t* buffer) override;
    virtual void readEndpoint(uint8_t number) override;
	virtual ~USBLowLevel_STM32F0();
//protected:
	void _usbISR() override;
private:
	void onCorrectTransfer();
};

} /* namespace USB */

#endif /* TARGET_STM32F0_USBLOWLEVEL_STM32F0_H_ */
