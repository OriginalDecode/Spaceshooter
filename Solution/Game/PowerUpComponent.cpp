#include "stdafx.h"

#include "CollisionNote.h"
#include "PowerUpComponent.h"
#include "PowerUpNote.h"

PowerUpComponent::PowerUpComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void PowerUpComponent::ReceiveNote(const PowerUpNote& aNote)
{
	//if (aMessage.GetType() == ePowerUpType::FIRERATEBOOST)
	//{
	//	//PostMaster::GetInstance()->SendMessage(msg);
	//}
}

void PowerUpComponent::ReceiveNote(const CollisionNote& aNote)
{
	
}