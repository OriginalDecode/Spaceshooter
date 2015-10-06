#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include <Engine.h>
#include "Entity.h"
#include <sstream>
#include <Vector.h>

AIComponent::AIComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
{

}

void AIComponent::Init()
{
	myEntityToFollow = nullptr;

	myTimeToNextDecision = 1.f;
	mySpeed = 80.f;
	myVelocity = myEntity.myOrientation.GetForward() * mySpeed;
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
			Shoot();
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
	CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

	myVelocity += toTarget * aDeltaTime;

	CU::Normalize(myVelocity);

	CU::Vector3<float> up(0, 1.f, 0);
	up = up * myEntity.myOrientation;

	CU::Normalize(toTarget);

	up = up + toTarget * aDeltaTime;

	CU::Normalize(up);

	CU::Vector3<float> right = CU::Cross(up, myVelocity);
	up = CU::Cross(myVelocity, right);

	right = CU::Cross(up, myVelocity);

	myEntity.myOrientation.myMatrix[0] = right.x;
	myEntity.myOrientation.myMatrix[1] = right.y;
	myEntity.myOrientation.myMatrix[2] = right.z;
	myEntity.myOrientation.myMatrix[3] = 0;

	myEntity.myOrientation.myMatrix[4] = up.x;
	myEntity.myOrientation.myMatrix[5] = up.y;
	myEntity.myOrientation.myMatrix[6] = up.z;
	myEntity.myOrientation.myMatrix[7] = 0;

	myEntity.myOrientation.myMatrix[8] = myVelocity.x;
	myEntity.myOrientation.myMatrix[9] = myVelocity.y;
	myEntity.myOrientation.myMatrix[10] = myVelocity.z;
	myEntity.myOrientation.myMatrix[11] = 0;

	myVelocity *= mySpeed;

	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() + aDeltaTime * myVelocity);
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