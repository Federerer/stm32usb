/*
 * SetupPacket.h
 *
 *  Created on: 17 sie 2017
 *      Author: 08fed
 */

#ifndef INCLUDE_SETUPPACKET_H_
#define INCLUDE_SETUPPACKET_H_

namespace USBLib
{

enum class DataPhaseTransferDirection
	: uint8_t
	{
		HostToDevice, DeviceToHost
};

enum class Type
	: uint8_t
	{
		Standard, Class, Vendor
};

enum class Recipient
	: uint8_t
	{
		Device, Interface, Endpoint, Other
};

enum class Request
	: uint8_t
	{
		GetStatus,
	ClearFeature,
	SetFeature = 3,
	SetAddress = 5,
	GetDescriptor,
	SetDescriptor,
	GetConfiguration,
	Setconfiguration,
	GetInterface,
	SetInterface
};

struct RequestType
{
	DataPhaseTransferDirection direction :1;
	Type type :2;
	Recipient recipient :5;
};

struct SetupPacket
{
	RequestType bmRequestType;
	Request bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;

	static inline SetupPacket deserialize(uint8_t* data)
	{
		RequestType r =
		{ (DataPhaseTransferDirection) ((data[0] & 0x80) >> 7), (Type) ((data[0]
				& 0x60) >> 5), (Recipient) (data[0] & 0x1f) };
		return
		{
			{	r},
			(Request)data[1],
			(uint16_t)(data[2] | (uint16_t) data[3] << 8),
			(uint16_t)(data[4] | (uint16_t) data[5] << 8),
			(uint16_t)(data[6] | (uint16_t) data[7] << 8)
		};
	}
};

} /* namespace stringDescriptors */

#endif /* INCLUDE_SETUPPACKET_H_ */
