#include "stdafx.h"

#include "CollisionComponent.h"
#include "Entity.h"

void CollisionComponent::Init()
{

}

void CollisionComponent::Update(float aDeltaTime)
{
	mySphere.myCenterPosition = myEntity->myOrientation.GetPos();
}

void CollisionComponent::ReceiveMessage(const RefreshOrientationMessage& aMessage)
{

}

void CollisionComponent::ReceiveMessage(const ShootMessage& aMessage)
{

}

