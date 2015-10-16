#include "stdafx.h"
#include "PowerUpMessage.h"


PowerUpMessage::PowerUpMessage(CU::Vector3<float> aPos, float aRadius, float aTime)
	: Message(eMessageType::POWER_UP)
	, myPosition(aPos)
	, myRadius(aRadius)
	, myTime(aTime)
{
}
