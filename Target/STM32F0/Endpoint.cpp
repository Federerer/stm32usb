/*
 * Endpoint.cpp
 *
 *  Created on: 15 sie 2017
 *      Author: 08fed
 */

#include "Endpoint.h"
#include "stm32f042x6.h"
#include <new>

namespace USBLib
{

constexpr uint8_t statRxBit = 12;
constexpr uint8_t statTxBit = 4;

volatile EndpointBufferDescription* endpointBufferDescriptionTable = new (
		reinterpret_cast<void*>(USB_PMAADDR)) EndpointBufferDescription[8];

const Endpoint endpoints[] =
{ 0, 1, 2, 3, 4, 5, 6, 7 };

Endpoint::Endpoint(uint8_t number) :
		_number
		{ number }
{

}

void Endpoint::handleInTransfer()
{
}

void Endpoint::openForTx(EndpointType type, uint16_t maxPacketSize) const
{
	auto reg = reinterpret_cast<uint16_t*>(USB + 4 * _number);

	// set up the endpoint type and address

	*reg = (*reg & USB_EP_T_MASK) | static_cast<uint16_t>(type);
	*reg = (*reg & USB_EPREG_MASK) | _number | USB_EP_CTR_TX;

	// set up the TX address in the BTABLE

	endpointBufferDescriptionTable[_number].tx.addr = 0x58;

	// if DTOG_TX is 1 then we need to write 1 to toggle it to zero

	if ((*reg & USB_EP_DTOG_TX) != 0)
		*reg = (*reg & USB_EPREG_MASK) | USB_EP_CTR_TX | USB_EP_DTOG_TX;

	// NAK the TX endpoint (nothing to go yet)

	setTxStatus(EndpointStatus::Naked);
}

void Endpoint::openForRx(EndpointType type, uint16_t maxPacketSize) const
{
	auto reg = getEndpointRegister();

	// set up the endpoint type and address

	*reg = (*reg & USB_EP_T_MASK) | static_cast<uint16_t>(type);
	*reg = (*reg & USB_EPREG_MASK) | _number | USB_EP_CTR_RX;

	// set up the RX address in the BTABLE and initialise first packet read

	endpointBufferDescriptionTable[_number].rx.addr = 0x18;
	endpointBufferDescriptionTable[_number].rx.setRxCount(maxPacketSize);

	// if DTOG_RX is 1 then we need to write 1 to toggle it to zero

	if ((*reg & USB_EP_DTOG_RX) != 0)
		*reg = (*reg & USB_EPREG_MASK) | USB_EP_CTR_RX | USB_EP_DTOG_RX;

	// inititate reception of the first packet

	setRxStatus(EndpointStatus::Valid);
}

void Endpoint::setTxStatus(EndpointStatus status) const
{
	auto reg = getEndpointRegister();
	auto sts = static_cast<uint8_t>(status) << statTxBit;

	uint16_t tmp;

	tmp = *reg & USB_EPTX_DTOGMASK;
	*reg = (tmp ^ sts) | USB_EP_CTR_RX | USB_EP_CTR_TX;
}

void Endpoint::setRxStatus(EndpointStatus status) const
{
	auto reg = getEndpointRegister();
	uint16_t sts = static_cast<uint8_t>(status) << statRxBit;

	uint16_t tmp;

	tmp = *reg & USB_EPRX_DTOGMASK;
	*reg = (tmp ^ sts) | USB_EP_CTR_RX | USB_EP_CTR_TX;
}

uint16_t Endpoint::readData(uint8_t* buffer, uint16_t bufferSize) const
{
	uint32_t i;

	auto destptr = reinterpret_cast<uint16_t *>(buffer);
	auto srcptr = reinterpret_cast<uint16_t *>(USB_PMAADDR
			+ endpointBufferDescriptionTable[_number].rx.addr);

	auto bytesToRead = endpointBufferDescriptionTable[_number].rx.getCount();

	for (i = (bytesToRead + 1) / 2; i > 0; i--)
		*destptr++ = *srcptr++;

	auto endpointReg = getEndpointRegister();

	*endpointReg = *endpointReg & ~(USB_EP_CTR_RX) & USB_EPREG_MASK;

	return bytesToRead;
}

bool Endpoint::write(const uint8_t* data, uint16_t len) const
{
	auto n = (len + 1) / 2;

	auto destptr = reinterpret_cast<uint16_t *>(USB_PMAADDR
			+ endpointBufferDescriptionTable[_number].tx.addr);

	for (auto i = n; i != 0; i--)
	{
		uint16_t word = data[0] | ((uint16_t) data[1] << 8);
		*destptr++ = word;
		data += 2;
	}

	endpointBufferDescriptionTable[_number].tx.count = len;
	setTxStatus(EndpointStatus::Valid);
}

void Endpoint::read() const
{
	endpointBufferDescriptionTable[_number].rx.setRxCount(64);
	setRxStatus(EndpointStatus::Valid);
}

Endpoint::~Endpoint()
{
	// TODO Auto-generated destructor stub
}

} /* namespace USB */
