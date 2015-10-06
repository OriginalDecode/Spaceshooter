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

	myMaxSpeed = 400;
	myMaxAcceleration = 40;
	mySlowRadius = 100;
	myTargetRadius = 50;
	myTimeToTarget = 1.f;
	myTimeToNextDecision = 1.f;
	mySpeed = 25.f;
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
	//CalcualteSteering(aDeltaTime);
	CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

	myVelocity += toTarget * aDeltaTime;

	CU::Normalize(myVelocity);

	CU::Vector3<float> right(1.f, 0, 0);
	right = right * myEntity.myOrientation;

	CU::Vector3<float> up = CU::Cross(myVelocity, right);

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
	CU::Vector3<float> toTarget3D = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

	toTarget3D = toTarget3D * myEntity.myOrientation;
	//myEntity.myOrientation.SetPos({ 0, 0, 0, 1 });
	CU::Vector3<float> forward3D = myEntity.myOrientation.GetForward();
	forward3D = forward3D * myEntity.myOrientation;

	forward3D.x = 0;
	forward3D.y = 0;
	forward3D.z = 1.f;

	std::stringstream ss1;
	std::stringstream ss2;
	ss1 << "Forward: " << forward3D.x << " " << forward3D.y << " " << forward3D.z << "\n";
	ss2 << "toTarget3D: " << toTarget3D.x << " " << toTarget3D.y << " " << toTarget3D.z << std::endl;

	//else //pitch
	{
		ss1 << "pitch";
		CU::Vector2<float> toTarget(toTarget3D.y, toTarget3D.z);

		CU::Normalize(toTarget);


		CU::Vector2<float> forward(forward3D.y, forward3D.z);

		CU::Normalize(forward);

		float dirAlpha = forward.x * toTarget.y - forward.y * toTarget.x;

		int dir = -1;

		if (dirAlpha > 0)
		{
			dir = 1;
		}

		CU::Matrix44<float> rotate = CU::Matrix44<float>::CreateRotateAroundX(acosf(CU::Clip(CU::Dot(forward, toTarget), -1.f, 1.f))
			* dir);

		myEntity.myOrientation = rotate * myEntity.myOrientation;
	}

	////if (fabs(toTarget3D.x) > fabs(toTarget3D.y)) // yaw
	//{
	//	ss1 << "yaw";
	//	CU::Vector2<float> toTarget(toTarget3D.x, toTarget3D.z);

	//	CU::Normalize(toTarget);


	//	CU::Vector2<float> forward(forward3D.x, forward3D.z);

	//	CU::Normalize(forward);

	//	float dirAlpha = forward.x * toTarget.y - forward.y * toTarget.x;

	//	int dir = 1;

	//	if (dirAlpha > 0)
	//	{
	//		dir = -1;
	//	}

	//	CU::Matrix44<float> rotate = CU::Matrix44<float>::CreateRotateAroundY(acosf(CU::Clip(CU::Dot(forward, toTarget), -1.f, 1.f))
	//		* dir * aDeltaTime);

	//	myEntity.myOrientation = rotate * myEntity.myOrientation;
	//}


	Prism::Engine::GetInstance()->PrintDebugText(ss1.str(), { 500.f, -500.f });
	Prism::Engine::GetInstance()->PrintDebugText(ss2.str(), { 500.f, -530.f });

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