#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(const eBulletType& aType, const CU::Vector3<float>& aVelocity, const CU::Matrix44<float>& anOrientation)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
	myType = aType;
	myVelocity = aVelocity;
	myOrientation = anOrientation;
}