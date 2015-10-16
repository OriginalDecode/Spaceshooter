#include "stdafx.h"
#include "BulletMessage.h"

BulletMessage::BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType, 
	const CU::Vector3<float>& aEnitityVelocity, const CU::Vector2<float>& anEntitySteering)
	: myType(aType)
	, myOrientation(anOrientation)
	, myEntityType(aEntityType)
	, myEntityVelocity(aEnitityVelocity)
	, myEntitySteering(anEntitySteering)
	, Message(eMessageType::ACTIVATE_BULLET)
{
}