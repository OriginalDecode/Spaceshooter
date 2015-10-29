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
#include "SoundNote.h"

PowerUpComponent::PowerUpComponent(Entity& aEntity)
	: Component(aEntity)
	, myPlayer(nullptr)
	, myDuration(0)
	, myValue(0)
	, myUpgradeName("")
	, myInGameName("")
{
}

void PowerUpComponent::Init(ePowerUpType someType, std::string anInGameName, float someValue, float someDuration)
{
	myType = someType;
	myValue = someValue;
	myDuration = someDuration;
	myInGameName = anInGameName;
}

void PowerUpComponent::Init(ePowerUpType someType, std::string anInGameName, std::string aUpgradeName, int anUpgradeID)
{
	myType = someType;
	myUpgradeID = anUpgradeID;
	myInGameName = anInGameName;
	myUpgradeName = aUpgradeName;

}

void PowerUpComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (myType == ePowerUpType::WEAPON_UPGRADE)
	{
		PostMaster::GetInstance()->SendMessage(PowerUpMessage(myType, myUpgradeName, myUpgradeID));
	}
	else
	{
		PowerUpNote note(myType, myValue, myDuration);
		aNote.myEntity.SendNote(note);
	}
	aNote.myEntity.SendNote<SoundNote>(SoundNote(eSoundNoteType::PLAY, "Play_PowerUpTaken"));
	aNote.myCollisionManager.Remove(myEntity.GetComponent<CollisionComponent>(), myEntity.GetType());
	myEntity.Kill();
}

void PowerUpComponent::Update(float aDeltaTime)
{
	myPlayer->SendNote<GUINote>(GUINote(&myEntity, eGUINoteType::POWERUP));

	CU::Vector3f pos = myEntity.myOrientation.GetPos();
	myEntity.myOrientation *= CU::Matrix44f::CreateRotateAroundZ(1.f* aDeltaTime);
	myEntity.myOrientation.SetPos(pos);
}

void PowerUpComponent::SetPlayer(Entity* aPlayer)
{
	myPlayer = aPlayer;
}
