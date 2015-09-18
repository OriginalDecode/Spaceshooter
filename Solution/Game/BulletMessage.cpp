#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
	myType = aType;
	myVelocity = aVelocity;
	myPosition = aPosition;
}

BulletMessage::~BulletMessage()
{
}