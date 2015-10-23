#include "stdafx.h"
#include <AudioInterface.h>
#include "BulletCollisionToGUIMessage.h"
#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "CollisionNote.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "PostMaster.h"
#include "ShieldComponent.h"

BulletComponent::BulletComponent(Entity& aEntity)
	: Component(aEntity)
{
	myCurrentLifeTime = 0.f;
	myDamageRadius = 0.f;
	SetActive(false);
}

void BulletComponent::Update(float aDeltaTime)
{
	if (myCurrentLifeTime >= myMaxLifeTime)
	{
		SetActive(false);
		myCurrentLifeTime = 0.f;
	}
	myCurrentLifeTime += aDeltaTime;
}

void BulletComponent::Init(float aMaxTime, int aDamage, float aDamageRadius, eBulletType aType)
{
	myType = aType;
	SetActive(false);
	myMaxLifeTime = aMaxTime;
	myDamage = aDamage;
	myDamageRadius = aDamageRadius;
	DL_ASSERT_EXP(myDamage >= 0, "Can't have negative damage.");
} 

void BulletComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (aNote.myEntity.GetAlive() == true)
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Stop_Rocket", myEntity.GetAudioSFXID());
		if (aNote.myEntity.GetComponent<ShieldComponent>() != nullptr)
		{
			COMPONENT_LOG("Shield component found on entity. (BulletComponent)");
			aNote.myEntity.GetComponent<ShieldComponent>()->DamageShield(myDamage);
		}

		if (aNote.myEntity.GetComponent<ShieldComponent>() == nullptr || 
				aNote.myEntity.GetComponent<ShieldComponent>()->GetCurrentShieldStrength() <= 0)
		{
			COMPONENT_LOG("No shield component found on entity or shield were depleted.");
			aNote.myEntity.GetComponent<HealthComponent>()->RemoveHealth(myDamage);
		}

		SetActive(false);
		aNote.myCollisionManager.Remove(myEntity.GetComponent<CollisionComponent>(), myEntity.GetType());
		
		if (myDamageRadius > 0.f)
		{
			aNote.myCollisionManager.DamageEnemiesWithinSphere(myEntity.myOrientation.GetPos(), myDamageRadius, myDamage);
		}

		PostMaster::GetInstance()->SendMessage<BulletCollisionToGUIMessage>(BulletCollisionToGUIMessage(this->GetEntity(), aNote.myEntity));
	}

}

void BulletComponent::SetActive(bool aActive)
{
	myActive = aActive;
	if (myActive == true)
	{
		if (myEntity.GetType() == eEntityType::PLAYER_BULLET)
		{
			if (myType == eBulletType::MACHINGUN_BULLET_LEVEL_1
				|| myType == eBulletType::MACHINGUN_BULLET_LEVEL_2
				|| myType == eBulletType::MACHINGUN_BULLET_LEVEL_3)
			{
				Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Laser", myEntity.GetAudioSFXID());
				//bullet->SendNote<SoundNote>(SoundNote(eSoundEvent::PLAY, )

			}
			if (myType == eBulletType::SHOTGUN_BULLET_LEVEL_1
				|| myType == eBulletType::SHOTGUN_BULLET_LEVEL_2
				|| myType == eBulletType::SHOTGUN_BULLET_LEVEL_3)
			{
				Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Shotgun", myEntity.GetAudioSFXID());
			}
			if (myType == eBulletType::ROCKET_MISSILE_LEVEL_1
				|| myType == eBulletType::ROCKET_MISSILE_LEVEL_2
				|| myType == eBulletType::ROCKET_MISSILE_LEVEL_3)
			{
				Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Rocket", myEntity.GetAudioSFXID());
			}
		}

	}
	else
	{
		//SEND STOPNOTE SOUND
	}
}