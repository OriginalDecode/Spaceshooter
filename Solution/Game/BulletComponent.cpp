#include "stdafx.h"
#include "BulletComponent.h"

BulletComponent::BulletComponent()
{
	myCurrentLifeTime = 0.f;
	myActive = false;
}

BulletComponent::~BulletComponent()
{
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