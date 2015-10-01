#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include <Vector.h>
#include "Entity.h"

AIComponent::AIComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
{

}

void AIComponent::Init()
{
	myEntityToFollow = nullptr;

	myMaxSpeed = 400;
	myMaxAcceleration = 40;
	mySlowRadius = 100;
	myTargetRadius = 50;
	myTimeToTarget = 1.f;
}

void AIComponent::Update(float aDeltaTime)
{
	if (myEntityToFollow == nullptr)
	{
		FollowOwnDecision(aDeltaTime);
	}
	else
	{
		FollowEntity(aDeltaTime);
	}
}

void AIComponent::SetEntityToFollow(Entity* aEntity)
{
	myEntityToFollow = aEntity;
}

void AIComponent::MakeDecision()
{
	myDecision = rand() % 9;
	myTimeToNextDecision = 1.f;
	if (myDecision == 2 || myDecision == 8)
	{
		Shoot();
	}
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	SteeringOutput steering = CalcualteSteering();

	Move(myVelocity * aDeltaTime);

	myVelocity += steering.myLinear * aDeltaTime;

	if (CU::Length(myVelocity) > myMaxSpeed)
	{
		CU::Normalize(myVelocity);
		myVelocity *= myMaxSpeed;
	}

	GetNewOrientation(myVelocity);

	SetRotation(CU::Matrix44<float>::CreateRotateAroundY(myOrientation.x));
}

void AIComponent::GetNewOrientation(const CU::Vector3<float>& aVelocity)
{
	if (CU::Length(aVelocity) > 0)
	{
		myOrientation.x = atan2(aVelocity.x, aVelocity.z);
	}
}

AIComponent::SteeringOutput AIComponent::CalcualteSteering()
{
	SteeringOutput output;


	CU::Vector3<float> direction = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
	float distance = CU::Length(direction);

	if (distance < myTargetRadius)
	{
		return output;
	}

	float targetSpeed;
	if (distance < mySlowRadius)
	{
		targetSpeed = myMaxSpeed;
	}
	else
	{
		targetSpeed = myMaxSpeed * distance / mySlowRadius;
	}

	CU::Vector3<float> targetVelocity = direction;
	CU::Normalize(targetVelocity);
	targetVelocity *= targetSpeed;

	output.myLinear = targetVelocity - myVelocity;
	output.myLinear /= myTimeToTarget;

	if (CU::Length(output.myLinear) > myMaxAcceleration)
	{
		CU::Normalize(output.myLinear);
		output.myLinear *= myMaxAcceleration;
	}

	output.myAngular = { 0, 0, 0 };

	return output;
}

void AIComponent::FollowOwnDecision(float aDeltaTime)
{
	myTimeToNextDecision -= aDeltaTime;
	if (myTimeToNextDecision <= 0.f)
	{
		MakeDecision();
	}

	switch (myDecision)
	{
	case 0:
		MoveLeft(10.f * aDeltaTime);
		break;
	case 1:
		MoveRight(10.f * aDeltaTime);
		break;
	case 2:
		MoveUp(10.f * aDeltaTime);
		break;
	case 3:
		MoveDown(10.f * aDeltaTime);
		break;
	case 4:
		MoveForward(10.f * aDeltaTime);
		break;
	case 5:
		MoveBackward(10.f * aDeltaTime);
		break;
	case 6:
		RotateX(globalPi / 4.f * aDeltaTime);
		break;
	case 7:
		RotateY(globalPi / 4.f * aDeltaTime);
		break;
	case 8:
		RotateZ(globalPi / 4.f * aDeltaTime);
		break;
	}
}