#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include <Vector.h>
#include "Entity.h"

void AIComponent::Init()
{
	myEntityToFollow = nullptr;
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
		Shoot(100.f);
	}
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	CU::Vector3<float> velocity;
	float maxSpeed = 50;
	float radius = 10;

	velocity = myEntityToFollow->myOrientation.GetPos() - myEntity->myOrientation.GetPos();

	if (CU::Length(velocity) < radius)
	{
		return;
	}

	float timeToTargetInSeconds = 1.f;

	velocity /= timeToTargetInSeconds;

	if (CU::Length(velocity) > maxSpeed)
	{
		CU::Normalize(velocity);
		velocity *= maxSpeed;
	}

	GetNewOrientation(velocity);

	SetRotation(CU::Matrix44<float>::CreateRotateAroundY(myOrientation.x));
	//RotateY(myOrientation);
	Move(velocity * aDeltaTime);
}

void AIComponent::GetNewOrientation(const CU::Vector3<float>& aVelocity)
{
	if (CU::Length(aVelocity) > 0)
	{
		myOrientation.x = atan2(aVelocity.x, aVelocity.z);
	}

	
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