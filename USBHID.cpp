/*
 * USBHID.cpp
 *
 *  Created on: 14 sie 2017
 *      Author: 08fed
 */

#include "USBHID.h"
#include "USBLowLevel.h"

namespace USBLib
{


USBHID::~USBHID()
{
	// TODO Auto-generated destructor stub
}

} /* namespace USB */


void USBLib::USBHID::connectStateChanged(bool connected)
{
}

void USBLib::USBHID::suspendStateChanged(bool suspended)
{
}

void USBLib::USBHID::SOF(int frameNumber)
{
}
