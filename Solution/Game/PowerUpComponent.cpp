#include "stdafx.h"

#include "CollisionComponent.h"
#include "CollisionNote.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "Enums.h"
#include <Instance.h>
#include "GUINote.h"
#include "GraphicsComponent.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PowerUpMessage.h"
#include "PowerUpNote.h"

PowerUpComponent::PowerUpComponent(Entity& aEntity)
	: Component(aEntity)
	, myPlayer(nullptr)
	, myDuration(0)
	, myShieldStrength(0)
	, myHealthRecover(0)
	, myFireRateMultiplier(0)
	, myUpgradeName("")
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

void PowerUpComponent::Init(ePowerUpType someType, float someDuration)
{
	myType = someType;
	myDuration = someDuration;
}

void PowerUpComponent::Init(ePowerUpType someType, std::string aUpgradeName)
{
	myType = someType;
	myUpgradeName = aUpgradeName;
}

void PowerUpComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (myType == ePowerUpType::WEAPON_UPGRADE)
	{
		PostMaster::GetInstance()->SendMessage(PowerUpMessage(myType, myUpgradeName));
	}
	else
	{
		PowerUpNote note(myType, myDuration, myShieldStrength, myHealthRecover, myFireRateMultiplier);
		aNote.myEntity.SendNote(note);
	}

	aNote.myCollisionManager.Remove(myEntity.GetComponent<CollisionComponent>(), myEntity.GetType());
	myEntity.Kill();
}

void PowerUpComponent::Update(float aDeltaTime)
{
	myPlayer->SendNote<GUINote>(GUINote(myEntity.myOrientation.GetPos(), eGUINoteType::POWERUP));

	CU::Vector3f pos = myEntity.myOrientation.GetPos();
	myEntity.myOrientation *= CU::Matrix44f::CreateRotateAroundZ(1.f* aDeltaTime);
	myEntity.myOrientation.SetPos(pos);
}

void PowerUpComponent::SetPlayer(Entity* aPlayer)
{
	myPlayer = aPlayer;
}
