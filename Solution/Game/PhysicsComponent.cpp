#include "stdafx.h"

#include "ControllerComponent.h"
#include "Entity.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity& aEntity)
	: Component(aEntity)
{
	myVelocity = { 0, 0, 0 };
}

void PhysicsComponent::Init(const CU::Matrix44<float>& anOrientation, const CU::Vector3<float>& aVelocity, unsigned short aWeight)
{
	myVelocity = aVelocity;
	myWeight = aWeight;
	myEntity.myOrientation = anOrientation;
}

void PhysicsComponent::Init(unsigned short aWeight, const CU::Vector3<float>& aVelocity)
{
	myVelocity = aVelocity;
	myWeight = aWeight;
}

void PhysicsComponent::Update(float aDeltaTime)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() + aDeltaTime * myVelocity);
	
	// object loses its speed, velocity approaches 0 every update
	myVelocity.x -= myVelocity.x * (aDeltaTime / 10);
	myVelocity.y -= myVelocity.y * (aDeltaTime / 10);
	myVelocity.z -= myVelocity.z * (aDeltaTime / 10);
}

void PhysicsComponent::MoveForward(const float& aDistance)
{
	myVelocity = (myEntity.myOrientation.GetForward() * (aDistance));
}

void PhysicsComponent::BounceOff(const float& anEntityWeight)
{
	//myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() - myVelocity * 2.f);
	//myVelocity = myVelocity * -anEntityWeight;
	//myEntity.GetComponent<ControllerComponent>()->ResetMovementSpeed();
}