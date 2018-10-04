/*
 * USBDeviceConfigration.h
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#ifndef USBDEVICECONFIGRATION_H_
#define USBDEVICECONFIGRATION_H_

#include <cstdint>
#include <array>
#include <utility>

#include "USBDefines.h"

namespace USBLib
{

namespace
{
constexpr uint8_t charConverter(const char c, int i)
{
	return i % 2 ? (uint8_t) 0 : (uint8_t) c;
}

template<unsigned N, typename T, T ... Nums>
constexpr const std::array<uint8_t, 2 * N + 2> stringDescriptorHelper(
		const char (&str)[N], std::integer_sequence<T, Nums...>)
{
	return
	{	(uint8_t)2*N+2,(uint8_t)DescriptorType::StringDescriptor,charConverter(str[Nums/2], Nums)...};
}
}

template<unsigned N>
constexpr const std::array<uint8_t, 2 * N + 2> toStringDescriptor(
		const char (&str)[N])
{
	return stringDescriptorHelper(str,
			std::make_integer_sequence<unsigned, N * 2>());
}

constexpr std::array<uint8_t, 18> makeDeviceDescriptor(uint16_t vid, uint16_t pid, uint16_t version)
{
	return {
	        0x12,       							/* bLength */
	        (uint8_t)(DescriptorType::DeviceDescriptor),/* bDescriptorType */
	        LSB(USBStandardVersion),           		/* bcdUSB (LSB) */
	        MSB(USBStandardVersion),           		/* bcdUSB (MSB) */
	        0x00,                           		/* bDeviceClass */
	        0x00,                           		/* bDeviceSubClass */
	        0x00,                           		/* bDeviceprotocol */
			Endpoint0MaxPacketSize,            		/* bMaxPacketSize0 */
	        (uint8_t)(LSB(vid)),                 	/* idVendor (LSB) */
	        (uint8_t)(MSB(vid)),                 	/* idVendor (MSB) */
	        (uint8_t)(LSB(pid)),                	/* idProduct (LSB) */
	        (uint8_t)(MSB(pid)),                	/* idProduct (MSB) */
	        (uint8_t)(LSB(version)),           		/* bcdDevice (LSB) */
	        (uint8_t)(MSB(version)),           		/* bcdDevice (MSB) */
	        ManufacturerStringDescriptorOffset,    	/* iManufacturer */
	        ProductStringDescriptorOffset,          /* iProduct */
	        SerialStringDescriptorOffset,           /* iSerialNumber */
	        0x01                            	    /* bNumConfigurations */
	};
}

class USBDeviceConfigration
{
public:
	virtual const std::pair<const uint8_t*,uint8_t> getDeviceDescriptor() const = 0;
	virtual const std::pair<const uint8_t*,uint8_t> getManufacturerStringDescriptor() const = 0;
	virtual const std::pair<const uint8_t*,uint8_t> getProductStringDescriptor() const = 0;
};

template<int N, int M>
class USBDeviceConfigrationT: public USBDeviceConfigration
{
public:
	constexpr USBDeviceConfigrationT(uint16_t vid, uint16_t pid,
			uint16_t version, const char (&manufacturer)[N], const char (&product)[M]) :
			_vid
			{ vid }, _pid
			{ pid }, _version
			{ version }, _deviceDescriptor
			{ makeDeviceDescriptor(vid, pid, version) },
			_manufacturerDescriptor{toStringDescriptor(manufacturer)},
			_productDescriptor{toStringDescriptor(product)}
	{

	}
	const std::pair<const uint8_t*,uint8_t> getDeviceDescriptor() const override
	{
		return {_deviceDescriptor.data(), _deviceDescriptor.size()};
	}
	const std::pair<const uint8_t*,uint8_t> getManufacturerStringDescriptor() const override
	{
		return {_manufacturerDescriptor.data(), _manufacturerDescriptor.size()};
	}
	const std::pair<const uint8_t*, uint8_t> getProductStringDescriptor() const override
	{
		return {_productDescriptor.data(), _productDescriptor.size()};
	}

private:
	const uint16_t _vid;
	const uint16_t _pid;
	const uint16_t _version;
	const std::array<uint8_t, 18> _deviceDescriptor;
	const std::array<uint8_t, 2*N+2> _manufacturerDescriptor;
	const std::array<uint8_t, 2*M+2> _productDescriptor;
};



template<int N, int M>
constexpr const USBDeviceConfigrationT<N,M> createDeviceConfiguration(uint16_t vid,
		uint16_t pid, uint16_t version, const char (&manufacturer)[N], const char (&product)[M])
{
	return USBDeviceConfigrationT<N,M>(vid, pid, version, manufacturer, product);
}

} /* namespace USB */

#endif /* USBDEVICECONFIGRATION_H_ */
