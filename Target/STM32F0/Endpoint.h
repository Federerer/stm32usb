/*
 * Endpoint.h
 *
 *  Created on: 15 sie 2017
 *      Author: 08fed
 */

#ifndef TARGET_STM32F0_ENDPOINT_H_
#define TARGET_STM32F0_ENDPOINT_H_

#include <cstdint>
#include <utility>
#include "stm32f042x6.h"

namespace USBLib
{

struct UsbBufferDescriptionTableEntry
{

	volatile uint16_t addr;
	volatile uint16_t count;

	/*
	 * Get just the count from the register
	 */

	uint16_t getCount() volatile
	{
		return count & 0x3ff;
	}

	/*
	 * Set the encoded RX count
	 */

	void setRxCount(int length) volatile
	{

		uint16_t wNBlocks;

		if ((length) > 62)
		{

			wNBlocks = length >> 5;
			if ((length & 0x1f) == 0)
				wNBlocks--;

			count = (wNBlocks << 10) | 0x8000;
		}
		else
		{

			wNBlocks = length >> 1;

			if ((length & 0x1) != 0)
				wNBlocks++;

			count = wNBlocks << 10;
		}
	}
};

struct EndpointBufferDescription
{
	volatile UsbBufferDescriptionTableEntry tx;
	volatile UsbBufferDescriptionTableEntry rx;
};

extern volatile EndpointBufferDescription* endpointBufferDescriptionTable;
//#define endpointBufferDescriptionTable	((volatile EndpointBufferDescription *)USB_PMAADDR)

enum class EndpointType : uint16_t
{
	Bulk = 0x0000,
	Control = 0x0200,
	Isochronous = 0x0400,
	Interrupt = 0x0600,
};

enum class EndpointStatus : uint16_t
{
	Disabled,
	Stalled,
	Naked,
	Valid
};

class Endpoint
{
public:
	Endpoint(uint8_t number);
	void handleInTransfer();
	void openForTx(EndpointType type, uint16_t maxPacketSize) const;
	void openForRx(EndpointType type, uint16_t maxPacketSize) const;
	void setTxStatus(EndpointStatus status) const;
	void setRxStatus(EndpointStatus status) const;

	void read() const;
	uint16_t readData(uint8_t* buffer, uint16_t bufferSize) const;
	bool write(const uint8_t* data, uint16_t len) const;

	inline volatile uint16_t* getEndpointRegister() const
	{
		return reinterpret_cast<volatile uint16_t*>(USB + 4 * _number);
	};

	virtual ~Endpoint();
private:
	uint8_t _number;
};

extern const Endpoint endpoints[];

} /* namespace USB */

#endif /* TARGET_STM32F0_ENDPOINT_H_ */
