#include "stdafx.h"
#include "SpawnExplosionMessage.h"


SpawnExplosionMessage::SpawnExplosionMessage(eMessageType anExplosionType, CU::Vector3f aPosition)
	: myPosition(aPosition)
	, Message(anExplosionType)
{
	DL_ASSERT_EXP((anExplosionType == eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH ||
		anExplosionType == eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH ||
		anExplosionType == eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH ||
		anExplosionType == eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH), 
		"Wrong message type. Has to be an explosion type! Check enum for Explosion type or ask Linus Skold.");
}


SpawnExplosionMessage::~SpawnExplosionMessage()
{
}
