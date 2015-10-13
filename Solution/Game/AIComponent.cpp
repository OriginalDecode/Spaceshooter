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
{

}

void AIComponent::Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName)
{
	myEntityToFollow = nullptr;

	myTargetName = aTargetName;

	myTimeToNextDecision = aTimeBetweenDecisions;
	myMovementSpeed = aSpeed;
	myVelocity = myEntity.myOrientation.GetForward() * myMovementSpeed;

	myCanMove = true;
}

void AIComponent::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myEntityToFollow != nullptr, "AI needs an entity to follow.");

	DL_ASSERT_EXP(myEntity.GetComponent<PhysicsComponent>() != nullptr, "AI component needs physics component for movement."); // remove later
	myEntity.GetComponent<PhysicsComponent>()->SetVelocity(myVelocity);

	if (myCanMove == true)
	{
		myTimeToNextDecision -= aDeltaTime;
		FollowEntity(aDeltaTime);

		CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

		CU::Normalize(toTarget);
		if (CU::Dot(myEntity.myOrientation.GetForward(), toTarget) > 0.72f && myTimeToNextDecision < 0)
		{
			myTimeToNextDecision = myTimeBetweenDecisions;
			Shoot(myEntity.GetComponent<PhysicsComponent>()->GetVelocity());
		}
	}
	else
	{
		myTimeBeforeMovement -= aDeltaTime;

		if (myTimeBeforeMovement <= 0.f)
		{
			myCanMove = true;
		}
	}
}

void AIComponent::SetEntityToFollow(Entity* aEntity)
{
	myEntityToFollow = aEntity;
	myFollowingEntity = true;
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	if (myFollowingEntity == true)
	{
		myTargetPosition = myEntityToFollow->myOrientation.GetPos();
	}

	CU::Vector3<float> toTarget;
	toTarget = myTargetPosition - myEntity.myOrientation.GetPos();

	if (CU::Length(toTarget) < 100.f)
	{
		CU::Vector3<float> toEntity = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
		if (CU::Length(toEntity) > 100.f)
		{
			myTargetPosition = myEntityToFollow->myOrientation.GetPos();
			myFollowingEntity = true;
		}
		else
		{
			myTargetPosition = myEntityToFollow->myOrientation.GetPos()/* + myEntityToFollow->myOrientation.GetForward() * 500.f*/;
			myTargetPosition.x += static_cast<float>(rand() % 1000) - 500.f;
			myTargetPosition.y += static_cast<float>(rand() % 1000) - 500.f;
			myTargetPosition.z += static_cast<float>(rand() % 1000) - 500.f;
			
			myFollowingEntity = false;
		}
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