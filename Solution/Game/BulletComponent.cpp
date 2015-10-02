#include "stdafx.h"
#include "BulletComponent.h"

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