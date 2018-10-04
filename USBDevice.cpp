/*
 * USBDevice.cpp
 *
 *  Created on: 13 sie 2017
 *      Author: 08fed
 */

#include "USBDevice.h"

namespace USBLib
{

const uint8_t _configurationDescriptor[41] =
		{

				// configuration descriptor header

				0x09,// bLength: Configuration Descriptor size
				0x02,   // bDescriptorType: Configuration
				sizeof(_configurationDescriptor),
				0,  // wTotalLength
				0x01,                           // bNumInterfaces
				0x01,                           // bConfigurationValue
				0x00,                           // iConfiguration
				0x80 | 32, // bmAttributes: bus powered
				100 / 2, // MaxPower 100 mA: this current is used for detecting Vbus

				// interface descriptor

				0x09,// bLength: Interface Descriptor size
				0x04, // bDescriptorType: Interface descriptor type
				0x00,                   // bInterfaceNumber: Number of Interface
				0x00,                    // bAlternateSetting: Alternate setting
				0x02,                           // bNumEndpoints
				0x03,                           // bInterfaceClass: CUSTOM_HID
				0x00,                  // bInterfaceSubClass : 1=BOOT, 0=no boot
				0x00,        // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
				0x00,                  // iInterface: Index of string descriptor

				// HID descriptor

				0x09,// bLength: CUSTOM_HID Descriptor size
				0x21,   // bDescriptorType: CUSTOM_HID
				0x11,
				0x01,             // bcdHID HID Class Spec release number (1.11)
				0x00,                   // bCountryCode: Hardware target country
				0x01,  // bNumDescriptors: Number of class descriptors to follow

				0x22,                           // bDescriptorType
				42,
				0, // wItemLength: Total length of Report descriptor

				// IN endpoint descriptor

				0x07,// bLength: Endpoint Descriptor size
				0x05,         // bDescriptorType
				0x81,  // bEndpointAddress: Endpoint Address (IN)
				0x03,                        // bmAttributes: Interrupt endpoint
				14, 0, // wMaxPacketSize: 2 Byte max
				0x01,                     // bInterval: Polling Interval (20 ms)

				// OUT endpoint descriptor

				0x07,// bLength: Endpoint Descriptor size
				0x05,         // bDescriptorType
				0x01, // bEndpointAddress: Endpoint Address (OUT)
				0x03,                        // bmAttributes: Interrupt endpoint
				10, 0, // wMaxPacketSize: 2 Bytes max
				0x01,                    // bInterval: Polling Interval (20 ms)
		};

USBDevice::~USBDevice()
{
	// TODO Auto-generated destructor stub
}

const uint8_t* const USBDevice::getDeviceDescriptor()
{
	return _config->getDeviceDescriptor().first;
}

void USBDevice::busReset(void)
{
	_address = 0;
}

void USBDevice::connect()
{
	_lowLevelDriver.connect();
}

void USBDevice::disconnect()
{
	_lowLevelDriver.disconnect();
}

void USBDevice::EP0setupCallback(void)
{
	bool res;
	uint8_t buffer[12];

	_lowLevelDriver.readSetupHeader(buffer);

	_transfer.initialize(SetupPacket::deserialize(buffer));

	if (_transfer.setupPacket.bmRequestType.type == Type::Standard)
	{
		res = handleStandardRequest();
	}
	else
	{
		//callback do wy¿szej warstwy device/class request
	}

	/* Data or status stage if applicable */
	if (_transfer.setupPacket.wLength > 0)
	{
		_transfer.stage = TransferStage::Data;

		if (_transfer.setupPacket.bmRequestType.direction
				== DataPhaseTransferDirection::DeviceToHost)
		{
			/* Check if we'll need to send a zero length packet at */
			/* the end of this transfer */
			if (_transfer.setupPacket.wLength > _transfer.remaining) //??? <
			{
				/* Device wishes to transfer less than host requested */
				if ((_transfer.remaining % 64) == 0)
				{
					/* Transfer is a multiple of EP0 max packet size */
					_transfer.isZeroLengthRequired = true;
				}
			}

			/* IN stage */
			handleControlDataInStage();
		}
		else
		{
			/* OUT stage */
			_lowLevelDriver.readEndpoint(0);
		}
	}
	else
	{
		/* Status stage */
		_lowLevelDriver.writeEndpoint(0, nullptr, 0);
	}

}

bool USBDevice::handleStandardRequest()
{
	switch (_transfer.setupPacket.bRequest)
	{
	case Request::GetStatus:
		return false;
	case Request::ClearFeature:
		return false;
	case Request::SetFeature:
		return false;
	case Request::SetAddress:
		return handleSetAddress();
	case Request::GetDescriptor:
		return handleGetDescriptor();
	case Request::GetConfiguration:
		return false;
	case Request::Setconfiguration:
		return false;
	case Request::GetInterface:
		return false;
	case Request::SetInterface:
		return false;
	default:
		return false;
	}
}

bool USBDevice::handleGetDescriptor()
{
	auto descType = static_cast<DescriptorType>(_transfer.setupPacket.wValue
			>> 8);

	switch (descType)
	{
	case DescriptorType::DeviceDescriptor:
	{
		auto deviceDescriptor = _config->getDeviceDescriptor();
		_transfer.data = deviceDescriptor.first;
		_transfer.remaining = deviceDescriptor.second;
		return true;
	}
	case DescriptorType::ConfigurationDescriptor:
	{
		_transfer.data = _configurationDescriptor;
		_transfer.remaining = sizeof(_configurationDescriptor);
		return true;
	}
	default:
		return false;
	}
}

bool USBDevice::handleSetAddress()
{
	_address = _transfer.setupPacket.wValue & 0x7F;
	return true;
}

void USBDevice::EP0in(void)
{
	bool res = handleControlIn();
}

void USBDevice::EP0out(void)
{
	bool res = handleControlOut();
}

bool USBDevice::handleControlIn()
{
	bool res = false;
	res = handleControlDataInStage();
	if (_address != 0)
	{
		_lowLevelDriver.setAddress(_address);
		_address = 0;
	}

}

bool USBDevice::handleControlOut()
{
	uint8_t buffer[64];

	auto cnt = _lowLevelDriver.readEndpointData(0, buffer);

	_lowLevelDriver.readEndpoint(0);
}

/* Control transfer data IN stage */
bool USBDevice::handleControlDataInStage()
{
	/* Check if transfer has completed (status stage transactions */
	/* also have transfer.remaining == 0) */
	if (_transfer.remaining == 0)
	{
		if (_transfer.isZeroLengthRequired)
		{
			/* Send zero length packet */
			_lowLevelDriver.writeEndpoint(0, nullptr, 0);
			_transfer.isZeroLengthRequired = false; //???
		}

		_transfer.stage = TransferStage::Status;
		_lowLevelDriver.readEndpoint(0);

		/* Completed */
		return true;
	}

	auto packetSize = _transfer.remaining;

	if (packetSize > 64)
	{
		packetSize = 64;
	}

	/* Write to endpoint */
	_lowLevelDriver.writeEndpoint(0, _transfer.data, packetSize);
	_lowLevelDriver.readEndpoint(0);

	/* Update transfer */
	_transfer.data += packetSize;
	_transfer.remaining -= packetSize;

	return true;
}

}/* namespace USB */
