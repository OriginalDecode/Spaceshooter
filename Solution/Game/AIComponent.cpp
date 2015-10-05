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
	myTimeToNextDecision = 1.f;
}

void AIComponent::Update(float aDeltaTime)
{
	if (myEntityToFollow == nullptr)
	{
		FollowOwnDecision(aDeltaTime);
	}
	else
	{
		myTimeToNextDecision -= aDeltaTime;
		FollowEntity(aDeltaTime);

		CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
		
		CU::Normalize(toTarget);
		if (CU::Dot(myEntity.myOrientation.GetForward(), toTarget) > 0.72f && myTimeToNextDecision < 0)
		{
			myTimeToNextDecision = 3.f;
			//Shoot();
		}
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
	CalcualteSteering(aDeltaTime);
	return;
/*
	Move(myVelocity * aDeltaTime);

	myVelocity += steering.myLinear * aDeltaTime;

	if (CU::Length(myVelocity) > myMaxSpeed)
	{
		CU::Normalize(myVelocity);
		myVelocity *= myMaxSpeed;
	}

	GetNewOrientation(myVelocity);

	SetRotation(CU::Matrix44<float>::CreateRotateAroundY(myOrientation.x));*/
}

void AIComponent::GetNewOrientation(const CU::Vector3<float>& aVelocity)
{
	//if (CU::Length(aVelocity) > 0)
	//{
	//	myOrientation.x = atan2(aVelocity.x, aVelocity.z);
	//}
}

void AIComponent::CalcualteSteering(float aDeltaTime)
{
	CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

	CU::Normalize(toTarget);
	
	toTarget = toTarget * myEntity.myOrientation;

	CU::Vector3<float> forward = myEntity.myOrientation.GetForward();

	forward = forward * myEntity.myOrientation;

	int dir = 1;

	if (toTarget.x < 0)
	{
		dir = -1;
	}

	CU::Matrix44<float> rotate = CU::Matrix44<float>::CreateRotateAroundY(acosf(CU::Dot(forward, toTarget))
		* dir);

	myEntity.myOrientation = rotate * myEntity.myOrientation;

	//if (CU::Length(output.myLinear) > myMaxAcceleration)
	//{
	//	CU::Normalize(output.myLinear);
	//	output.myLinear *= myMaxAcceleration;
	//}
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