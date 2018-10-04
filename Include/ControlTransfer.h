/*
 * ControlTransfer.h
 *
 *  Created on: 17 sie 2017
 *      Author: 08fed
 */

#ifndef INCLUDE_CONTROLTRANSFER_H_
#define INCLUDE_CONTROLTRANSFER_H_

#include <cstdint>
#include "SetupPacket.h"

namespace USBLib
{

enum TransferStage : uint8_t
{
	Setup,
	Data,
	Status
};

struct ControlTransfer
{
	SetupPacket setupPacket;
	const uint8_t* data;
	uint16_t remaining;
	uint8_t direction;
	TransferStage stage;
	bool isZeroLengthRequired;
	bool notifyUpperLayer;

	inline void initialize(SetupPacket packet)
	{
		setupPacket = packet;
		data = nullptr;
		remaining = 0;
		direction = 0;
		stage = TransferStage::Setup;
		isZeroLengthRequired = false;
		notifyUpperLayer = false;
	}

};

} /* namespace stringDescriptors */

#endif /* INCLUDE_CONTROLTRANSFER_H_ */
