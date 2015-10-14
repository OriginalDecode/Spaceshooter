#include "stdafx.h"
#include "SpawnEnemyMessage.h"


SpawnEnemyMessage::SpawnEnemyMessage(const std::string& aType, const CU::Vector3<float>& aPosition, const CU::Vector3<float>& aRotation
		, const CU::Vector3<float>& aScale)
	: myType(aType)
	, myPosition(aPosition) 
	, myRotation(aRotation)
	, myScale(aScale)
	, Message(eMessageType::SPAWN_ENEMY)
{
}