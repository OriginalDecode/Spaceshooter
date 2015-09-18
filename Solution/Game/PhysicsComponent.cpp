#include "stdafx.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
	myDirection = { 0, 0, 0 };
	myVelocity = { 0, 0, 0 };
	myPosition = { 0, 0, 0 };
//	myID = 3;
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Init(const CU::Vector3<float> aVelocity, const CU::Vector3<float> aPosition)
{
	myVelocity = aVelocity;
	myPosition = aPosition;
}

void PhysicsComponent::Update(float aDeltaTime)
{
	myPosition += aDeltaTime * myVelocity;
}

//void PhysicsComponent::ReceiveMessage(eMessage aMessage)
//{
//	aMessage;
//}