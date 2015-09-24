#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(const eBulletType& aType, const CU::Matrix44<float>& anOrientation)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
	myType = aType;
	myOrientation = anOrientation;
}