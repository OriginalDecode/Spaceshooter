#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include <Vector.h>
#include "Entity.h"
#include "RefreshOrientationMessage.h"

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
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	CU::Vector3<float> newForward = CU::GetNormalized(
		myEntityToFollow->myOrientation.GetPos() - myEntity->myOrientation.GetPos());
	CU::Vector3<float> rotAxis = CU::Cross(myEntity->myOrientation.GetForward(), newForward);

	float rotAngle = acosf(CU::Dot(myEntity->myOrientation.GetForward(), newForward)) * aDeltaTime;

	Rotate(CU::Matrix44<float>::RotatePointAroundAxis(myEntity->myOrientation.GetPos(), rotAxis, rotAngle));

	MoveForward(50.f * aDeltaTime);
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