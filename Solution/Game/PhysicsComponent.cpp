#include "stdafx.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "RefreshOrientationMessage.h"

PhysicsComponent::PhysicsComponent()
{
	myDirection = { 0, 0, 0 };
	myVelocity = { 0, 0, 0 };
	myPosition = { 0, 0, 0 };
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Init(const CU::Vector3<float> aVelocity, const CU::Vector3<float> aPosition)
{
	myVelocity = aVelocity;
	myPosition = aPosition;
	myEntity->myOrientation.SetPos(myPosition);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void PhysicsComponent::Update(float aDeltaTime)
{
	myPosition += aDeltaTime * myVelocity;
	myEntity->myOrientation.SetPos(myPosition);
	myEntity->SendMessage(RefreshOrientationMessage());
}