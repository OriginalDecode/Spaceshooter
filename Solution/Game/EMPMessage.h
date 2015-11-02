#pragma once
#include "Message.h"


class EMPMessage : public Message
{
public:
	EMPMessage(float aEMPTime);

	const float myEMPTime;
};

