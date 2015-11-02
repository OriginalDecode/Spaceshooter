#include "stdafx.h"
#include "EMPMessage.h"


EMPMessage::EMPMessage(float aEMPTime)
	: Message(eMessageType::EMP)
	, myEMPTime(aEMPTime)
{
}
