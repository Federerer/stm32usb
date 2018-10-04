/*
 * USBDefines.h
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#ifndef INCLUDE_USBDEFINES_H_
#define INCLUDE_USBDEFINES_H_

#include <cstdint>

namespace USBLib
{

constexpr uint8_t MSB(uint16_t val)
{
	return (val & 0xFF00) >> 8;
}

constexpr uint8_t LSB(uint16_t val)
{
	return (val & 0xFF);
}

/* USB Standard version */

constexpr uint16_t USBStandardVersion = 0x0200;

/*string descriptor offset*/
constexpr uint8_t LangIdStringDescriptorOffset = 0;
constexpr uint8_t ManufacturerStringDescriptorOffset = 1;
constexpr uint8_t ProductStringDescriptorOffset = 2;
constexpr uint8_t SerialStringDescriptorOffset = 3;
constexpr uint8_t ConfigurationStringDescriptorOffset = 4;
constexpr uint8_t InterfaceStringDescriptorOffset = 5;

constexpr uint8_t Endpoint0MaxPacketSize = 64;

enum class DescriptorType
	: uint8_t
{
	DeviceDescriptor = 1,
	ConfigurationDescriptor,
	StringDescriptor,
	InterfaceDescriptor,
	EndpointDescriptor,
	QualifierDescriptor
};

}

#endif /* INCLUDE_USBDEFINES_H_ */
