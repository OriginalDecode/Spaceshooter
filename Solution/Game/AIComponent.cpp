#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include <Engine.h>
#include "Entity.h"
#include "PhysicsComponent.h"
#include <sstream>
#include <Vector.h>

AIComponent::AIComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
	, myPhysicsComponent(nullptr)
	, myAvoidanceDistance(300.f)
{

}

void AIComponent::Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName
	, float aAvoidanceDistance, const CU::Vector3<float>& aAvoidancePoint)
{
	myEntityToFollow = nullptr;

	myTargetName = aTargetName;

	myTimeToNextDecision = aTimeBetweenDecisions;
	myMovementSpeed = aSpeed;
	myVelocity = myEntity.myOrientation.GetForward() * myMovementSpeed;

	myCanMove = true;

	myAvoidanceDistance = aAvoidanceDistance;
	myFollowingOffset = aAvoidancePoint;
}

void AIComponent::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myEntityToFollow != nullptr, "AI needs an entity to follow.");

	if (myPhysicsComponent == nullptr)
	{
		myPhysicsComponent = myEntity.GetComponent<PhysicsComponent>();
		DL_ASSERT_EXP(myPhysicsComponent != nullptr, "AI component needs physics component for movement."); // remove later
	}

	myVelocity = myPhysicsComponent->GetVelocity();
	

	if (myCanMove == true)
	{
		myTimeToNextDecision -= aDeltaTime;
		FollowEntity(aDeltaTime);

		CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

		CU::Normalize(toTarget);
		if (CU::Dot(myEntity.myOrientation.GetForward(), toTarget) > 0.72f && myTimeToNextDecision < 0)
		{
			myTimeToNextDecision = myTimeBetweenDecisions;
			Shoot(myPhysicsComponent->GetVelocity());
		}
	}
	else
	{
		myVelocity -= myVelocity * aDeltaTime * 0.01f;
		myTimeBeforeMovement -= aDeltaTime;

		if (myTimeBeforeMovement <= 0.f)
		{
			myCanMove = true;
		}

		RotateX(aDeltaTime / 10);
		RotateZ(aDeltaTime / 5);
	}

	myPhysicsComponent->SetVelocity(myVelocity);
}

void AIComponent::SetEntityToFollow(Entity* aEntity)
{
	myEntityToFollow = aEntity;
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
	float distToTarget = CU::Length(toTarget);

	if (distToTarget < myAvoidanceDistance)
	{
		float distCoef = 1.f - (distToTarget / myAvoidanceDistance);
		toTarget += myFollowingOffset * distCoef;
	}


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

	myVelocity *= myMovementSpeed;
}