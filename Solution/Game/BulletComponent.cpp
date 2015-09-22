#include "stdafx.h"
#include "BulletComponent.h"

BulletComponent::BulletComponent()
{
	myCurrentLifeTime = 0.f;
	myIsActive = false;
}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::Update(float aDeltaTime)
{
	if (myCurrentLifeTime >= myMaxLifeTime)
	{
		myIsActive = false;
		myCurrentLifeTime = 0.f;
	}
	myCurrentLifeTime += aDeltaTime;
}