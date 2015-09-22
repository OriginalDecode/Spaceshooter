#include "stdafx.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "RefreshOrientationMessage.h"

PhysicsComponent::PhysicsComponent()
{
	myVelocity = { 0, 0, 0 };
	myPosition = { 0, 0, 0 };
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Init(const CU::Vector3<float> aVelocity, const CU::Vector3<float> aPosition, const CU::Vector3<float> aForward)
{
	myVelocity = aVelocity;
	myPosition = aPosition;
	myEntity->myOrientation.SetPos(myPosition);
	myEntity->myOrientation.myMatrix[8] = aForward.x;
	myEntity->myOrientation.myMatrix[9] = aForward.y;
	myEntity->myOrientation.myMatrix[10] = aForward.z;
	myEntity->SendMessage(RefreshOrientationMessage());
}

void PhysicsComponent::Update(float aDeltaTime)
{
	myPosition += aDeltaTime * myVelocity;
	myEntity->myOrientation.SetPos(myPosition);
	myEntity->SendMessage(RefreshOrientationMessage());
}