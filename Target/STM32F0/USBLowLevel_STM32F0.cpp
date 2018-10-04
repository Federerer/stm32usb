/*
 * USBLowLevel_STM32F0.cpp
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#include "USBLowLevel_STM32F0.h"
#include "USBDevice.h"
#include "Endpoint.h"

#include "stm32f042x6.h"

namespace USBLib
{

USBLowLevel_STM32F0::USBLowLevel_STM32F0()
{
	// TODO Auto-generated constructor stub

}

USBLowLevel_STM32F0::~USBLowLevel_STM32F0()
{
	// TODO Auto-generated destructor stub
}

void USBLowLevel_STM32F0::_usbISR()
{
	// correct endpoint transfer
	if ((USB->ISTR & USB_ISTR_CTR) != 0)
	{
		onCorrectTransfer();
	}

	// reset
	if ((USB->ISTR & USB_ISTR_RESET) != 0)
	{
		USB->ISTR &= ~USB_ISTR_RESET;

		USB->DADDR = USB_DADDR_EF;

		endpoints[0].openForTx(EndpointType::Control, 64);
		endpoints[0].openForRx(EndpointType::Control, 64);

		_deviceDriver->busReset();
	}

	// packet memory over/underflow

	if ((USB->ISTR & USB_ISTR_PMAOVR) != 0)
		//onPacketMemoryOverUnderFlow();

		// generic error

		if ((USB->ISTR & USB_ISTR_ERR) != 0)
			//onError();

			// wakeup

			if ((USB->ISTR & USB_ISTR_WKUP) != 0)
				//onWakeup();

				// suspend

				if ((USB->ISTR & USB_ISTR_SUSP) != 0)
					//onSuspend();

					// start of frame

					if ((USB->ISTR & USB_ISTR_SOF) != 0)
						//onStartOfFrame();

						// expected start of frame

						if ((USB->ISTR & USB_ISTR_ESOF) != 0)
							;
	//onExpectedStartOfFrame();
}

void USBLowLevel_STM32F0::onCorrectTransfer()
{
	uint16_t irq;
	uint8_t endpointIndex;
	bool isOut;
	volatile uint16_t *reg;

	// USB_ISTR_CTR is read only and will be automatically cleared by hardware
	// when we've processed all endpoint results

	while (((irq = USB->ISTR) & USB_ISTR_CTR) != 0)
	{
		endpointIndex = irq & USB_ISTR_EP_ID;
		isOut = (irq & USB_ISTR_DIR) != 0;

		if (endpointIndex == 0)
		{
			// control endpoint events
			if (isOut)
			{
				if ((USB->EP0R & USB_EP_SETUP) != 0)
				{
					USB->EP0R = USB->EP0R & ~(USB_EP_CTR_RX) & USB_EPREG_MASK;
					_deviceDriver->EP0setupCallback();
				}
				else if ((USB->EP0R & USB_EP_CTR_RX) != 0)
				{
					USB->EP0R = USB->EP0R & ~(USB_EP_CTR_RX) & USB_EPREG_MASK;
					_deviceDriver->EP0out();
				}

			}
			else
			{
				USB->EP0R = USB->EP0R & (~USB_EP_CTR_TX) & USB_EPREG_MASK;
				_deviceDriver->EP0in();
			}
		}
		else
		{
			// normal endpoint events

//			reg = &USB->EP0R + 2 * endpointIndex;

//			if ((*reg & USB_EP_CTR_RX) != 0)
//				handleEndpointOutTransfer(reg, endpointIndex);

//			    _deviceDriver->
//
//			if ((*reg & USB_EP_CTR_TX) != 0)
//				handleEndpointInTransfer(reg, endpointIndex);
		}
	}
}

bool USBLowLevel_STM32F0::connect()
{
	// set address to zero and default state

	//_address = 0;
	//setDeviceState(DeviceStateType::DEFAULT);
	//_remoteWakeup = false;

	// the buffer description table is at address 0 in the packet memory

	USB->BTABLE = 0;

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,ENABLE);
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;

	// Nvic::configureIrq(USB_IRQn);
	NVIC_EnableIRQ(USB_IRQn);

	// reset the peripheral and any pending interrupts

	//reset();
	USB->CNTR = USB_CNTR_FRES;
	USB->CNTR = 0;
	//clearInterrupts();
	USB->ISTR = 0;
	// set the interrupt mask

	USB->CNTR = USB_CNTR_CTRM          // correct transfer
	//| USB_CNTR_WKUPM        // wakeup
	//		| USB_CNTR_SUSPM        // suspend
	//		| USB_CNTR_ERRM         // error
	//		| USB_CNTR_ESOFM        // expected start of frame
			| USB_CNTR_RESETM;      // reset

	USB->BCDR |= USB_BCDR_DPPU;

	// notify

	//UsbStatus(UsbStatusType::STATE_DEFAULT);
}

bool USBLowLevel_STM32F0::disconnect()
{
	return false;
}

bool USBLowLevel_STM32F0::readSetupHeader(uint8_t* buffer)
{
	endpoints[0].readData(buffer, 64);
}

bool USBLowLevel_STM32F0::writeEndpoint(uint8_t number, const uint8_t* data,
		uint16_t len)
{
	endpoints[number].write(data, len);
}

uint16_t USBLowLevel_STM32F0::readEndpointData(uint8_t number, uint8_t* buffer)
{
	return endpoints[number].readData(buffer, 64);
}

void USBLowLevel_STM32F0::readEndpoint(uint8_t number)
{
	endpoints[number].read();
}

bool USBLowLevel_STM32F0::setAddress(uint8_t address)
{
	USB->DADDR = address | USB_DADDR_EF;
	return true;
}

} /* namespace USB */


