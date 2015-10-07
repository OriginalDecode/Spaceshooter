#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType, const float& aEnititySpeed)
	: myType(aType)
	, myOrientation(anOrientation)
	, myEntityType(aEntityType)
	, mySpeedMultiplier(aEnititySpeed)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
}