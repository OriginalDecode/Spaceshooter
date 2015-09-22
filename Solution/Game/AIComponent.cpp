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
	CU::Vector3<float> posDif = myEntity->myOrientation.GetPos() - myEntityToFollow->myOrientation.GetPos();
	if (CU::Length2(posDif) == 0.f)
	{
		return;
	}

	if (posDif.x > 0.2f)
	{
		RotateY(-globalPi / 4.f * aDeltaTime);
	}
	else if (posDif.x < -0.2f)
	{
		RotateY(globalPi / 4.f * aDeltaTime);
	}

	if (posDif.y > 0.2f)
	{
		RotateX(-globalPi / 4.f * aDeltaTime);
	}
	else if (posDif.y < -0.2f)
	{
		RotateX(globalPi / 4.f * aDeltaTime);
	}

	//if (posDif.z > 0.2f)
	//{
	//	RotateZ(-globalPi / 4.f * aDeltaTime);
	//}
	//else if (posDif.z < -0.2f)
	//{
	//	RotateZ(globalPi / 4.f * aDeltaTime);
	//}

	MoveForward(10.f * aDeltaTime);
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