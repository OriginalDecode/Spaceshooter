#include "stdafx.h"
#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "CollisionNote.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "ShieldComponent.h"

BulletComponent::BulletComponent(Entity& aEntity)
	: Component(aEntity)
{
	myCurrentLifeTime = 0.f;
	myActive = false;
}

void BulletComponent::Update(float aDeltaTime)
{
	if (myCurrentLifeTime >= myMaxLifeTime)
	{
		myActive = false;
		myCurrentLifeTime = 0.f;
	}
	myCurrentLifeTime += aDeltaTime;
}

void BulletComponent::Init(const float& aMaxTime, const unsigned short& aDamage)
{
	myActive = false;
	myMaxLifeTime = aMaxTime;
	myDamage = aDamage;
} 

void BulletComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (aNote.myEntity.GetAlive() == true)
	{
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

		myActive = false;
		aNote.myCollisionManager.Remove(myEntity.GetComponent<CollisionComponent>(), myEntity.GetType());

		//aNote.myCollisionManager.DamageEnemiesWithinSphere(myEntity.myOrientation.GetPos(), 500.f, myDamage);
	}
}