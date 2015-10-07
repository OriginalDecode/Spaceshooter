#include "stdafx.h"

#include "CollisionNote.h"
#include "Entity.h"
#include "PowerUpComponent.h"
#include "PowerUpNote.h"

PowerUpComponent::PowerUpComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void PowerUpComponent::Init(ePowerUpType someType)
{
	myType = someType;
}


void PowerUpComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (myType == ePowerUpType::FIRERATEBOOST)
	{
		PowerUpNote note(myType, 20, 0, 0, 3);
		aNote.myEntity.SendNote(note);
	}
	if (myType == ePowerUpType::HEALTHKIT_01)
	{

	}
	if (myType == ePowerUpType::HEALTHKIT_02)
	{

	}
	if (myType == ePowerUpType::SHIELDBOOST)
	{

	}
}