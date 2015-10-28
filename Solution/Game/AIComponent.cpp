#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include "DefendMessage.h"
#include <Engine.h>
#include "Entity.h"
#include <MathHelper.h>
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include <sstream>
#include <Vector.h>


AIComponent::AIComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
	, myPhysicsComponent(nullptr)
	, myAvoidanceDistance(300.f)
	, myPrevEntityToFollow(nullptr)
{
	DL_ASSERT_EXP(aEntity.GetComponent<InputComponent>() == nullptr, "Tried to add AIComponent when there was a InputComponent");
}

AIComponent::~AIComponent()
{
	if (myTargetPositionMode != eAITargetPositionMode::KAMIKAZE)
	{
		if (PostMaster::GetInstance()->IsSubscribed(eMessageType::DEFEND, this) == true)
		{
			PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
		}
	}
}

void AIComponent::Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName
	, float aAvoidanceDistance, const CU::Vector3<float>& aAvoidancePoint
	, eAITargetPositionMode aTargetPositionMode)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	myEntityToFollow = nullptr;
	myTargetName = aTargetName;

	myTimeToNextDecision = aTimeBetweenDecisions;
	
	PhysicsComponent* physicsComponent = myEntity.GetComponent<PhysicsComponent>();
	if (physicsComponent != nullptr)
	{
		physicsComponent->SetVelocity(myEntity.myOrientation.GetForward() * aSpeed);
	}

	myCanMove = true;

	myAvoidanceDistance = aAvoidanceDistance;
	myFollowingOffset = aAvoidancePoint;

	myTargetPositionMode = aTargetPositionMode;

	DL_ASSERT_EXP(myTargetPositionMode != eAITargetPositionMode::NOT_USED, "No AIMode was set!");
}

void AIComponent::Init(float aSpeed, eAITargetPositionMode aTargetPositionMode)
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	myTargetPositionMode = aTargetPositionMode;
	PhysicsComponent* physicsComponent = myEntity.GetComponent<PhysicsComponent>();
	if (physicsComponent != nullptr)
	{
		physicsComponent->SetVelocity(myEntity.myOrientation.GetForward() * aSpeed);
	}
	myEntityToFollow = nullptr;
	myTimeToNextDecision = 0.f;
	myAvoidanceDistance = 0.f;
	myFollowingOffset = CU::Vector3<float>();
	myCanMove = true;
	DL_ASSERT_EXP(myTargetPositionMode != eAITargetPositionMode::NOT_USED, "No AIMode was set!");
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

		if (myTargetPositionMode != eAITargetPositionMode::KAMIKAZE 
			&& myTargetPositionMode != eAITargetPositionMode::MINE)
		{
			CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

			CU::Normalize(toTarget);
			if (myTimeToNextDecision < 0)
			{
				myTimeToNextDecision = myTimeBetweenDecisions;
				/*PhysicsComponent* targetPhys = myEntityToFollow->GetComponent<PhysicsComponent>();
				CU::Vector3<float> targetVel = targetPhys->GetVelocity();*/

				Shoot(toTarget*myPhysicsComponent->GetSpeed());
			}
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

void AIComponent::SetEntityToFollow(Entity* aEntity, Entity* aPlayerEntity)
{
	myEntityToFollow = aEntity;
	if (myPrevEntityToFollow == nullptr)
	{
		myPrevEntityToFollow = aPlayerEntity;
	}
}

void AIComponent::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myPrevEntityToFollow = myEntityToFollow;
		myEntityToFollow = aMessage.myEntity;
	}
	else if (aMessage.myType == DefendMessage::eType::COMPLETE)
	{
		myEntityToFollow = myPrevEntityToFollow;
	}
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	if (myTargetPositionMode != eAITargetPositionMode::MINE)
	{
		CalculateToTarget(myTargetPositionMode);
		
		CU::Normalize(myToTarget);
		float turnModifier = 1.0f;

		myVelocity += myToTarget * turnModifier;

		CU::Normalize(myVelocity);

		CU::Vector3<float> up(myEntity.myOrientation.GetUp());

		

		up = up + myToTarget * aDeltaTime;

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

		myVelocity *= myPhysicsComponent->GetSpeed();
	}
	else
	{
		DL_ASSERT("myTargetPositionMode is MINE");
	}
}

void AIComponent::CalculateToTarget(eAITargetPositionMode aMode)
{
	if (aMode == eAITargetPositionMode::KEEP_DISTANCE)
	{
		myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
		float distToTarget = CU::Length(myToTarget);

		if (distToTarget < myAvoidanceDistance)
		{
			float distCoef = 1.f - (distToTarget / myAvoidanceDistance);
			myToTarget += myFollowingOffset * distCoef;
		}
	}

	else if (aMode == eAITargetPositionMode::ESCAPE_THEN_RETURN)
	{
		if (myIsEscaping == false)
		{
			myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
			float distToTarget = CU::Length(myToTarget);

			if (distToTarget < myAvoidanceDistance)
			{
				float distCoef = 1.f - (distToTarget / (myAvoidanceDistance));
				myToTarget += myFollowingOffset * distCoef;
				myEscapePosition = myEntityToFollow->myOrientation.GetPos() + CU::Math::RandomVector(-myFollowingOffset, myFollowingOffset);
				myIsEscaping = true;
			}
		}
		else
		{
			myToTarget = myEscapePosition - myEntity.myOrientation.GetPos();
			float distToTarget = CU::Length(myToTarget);

			if (distToTarget < 20)
			{
				myIsEscaping = false;
			}
		}
	}
	else if (aMode == eAITargetPositionMode::KAMIKAZE)
	{
		myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
	}
}
