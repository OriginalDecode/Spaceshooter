#include "stdafx.h"

#include "Entity.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity& aEntity)
	: Component(aEntity)
{
	myVelocity = { 0, 0, 0 };
}

void PhysicsComponent::Init(const CU::Matrix44<float>& anOrientation, const CU::Vector3<float> aVelocity)
{
	myVelocity = aVelocity;
	myEntity.myOrientation = anOrientation;
}

void PhysicsComponent::Update(float aDeltaTime)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() + aDeltaTime * myVelocity);
}