#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType)
	: myType(aType)
	, myOrientation(anOrientation)
	, myEntityType(aEntityType)
{
	myMessageType = eMessageType::ACTIVATE_BULLET;
}