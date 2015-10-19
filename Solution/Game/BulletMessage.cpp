#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType, 
	const CU::Vector3<float>& aEnitityVelocity)
	: myType(aType)
	, myOrientation(anOrientation)
	, myEntityType(aEntityType)
	, myEntityVelocity(aEnitityVelocity)
	, Message(eMessageType::ACTIVATE_BULLET)
{
}