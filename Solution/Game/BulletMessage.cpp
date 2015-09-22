#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition, CU::Vector3<float> aForward)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
	myType = aType;
	myVelocity = aVelocity;
	myPosition = aPosition;
	myForward = aForward;
}

BulletMessage::~BulletMessage()
{
}
