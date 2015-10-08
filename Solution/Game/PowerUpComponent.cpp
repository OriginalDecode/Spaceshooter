#include "stdafx.h"

#include "CollisionComponent.h"
#include "CollisionNote.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "Enums.h"
#include "GUINote.h"
#include "PowerUpComponent.h"
#include "PowerUpNote.h"

PowerUpComponent::PowerUpComponent(Entity& aEntity)
	: Component(aEntity)
	, myPlayer(nullptr)
{
}

void PowerUpComponent::Init(ePowerUpType someType, float someDuration, int someShieldStrength
	, int someHealthRecover, int someFireRateMultiplier)
{
	myType = someType;
	myDuration = someDuration;
	myShieldStrength = someShieldStrength;
	myHealthRecover = someHealthRecover;
	myFireRateMultiplier = someFireRateMultiplier;
}

void PowerUpComponent::ReceiveNote(const CollisionNote& aNote)
{
	PowerUpNote note(myType, myDuration, myShieldStrength, myHealthRecover, myFireRateMultiplier);

	aNote.myEntity.SendNote(note);

	aNote.myCollisionManager.Remove(myEntity.GetComponent<CollisionComponent>(), myEntity.GetType());
	myEntity.Kill();
}

void PowerUpComponent::Update(float aDeltaTime)
{
	myPlayer->SendNote<GUINote>(GUINote(myEntity.myOrientation.GetPos(), eGUINoteType::POWERUP));
}

void PowerUpComponent::SetPlayer(Entity* aPlayer)
{
	myPlayer = aPlayer;
}
