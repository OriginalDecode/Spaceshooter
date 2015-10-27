#pragma once

#include "ControllerComponent.h"
#include "Enums.h"
#include "Subscriber.h"

class PhysicsComponent;

class AIComponent : public ControllerComponent, public Subscriber
{
public:
	AIComponent(Entity& aEntity);
	~AIComponent();

	void Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName
		, float aAvoidanceDistance, const CU::Vector3<float>& aAvoidancePoint
		, eAITargetPositionMode aTargetPositionMode);

	void Init(float aSpeed, eAITargetPositionMode aTargetPositionMode);

	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity, Entity* aPlayerEntity);

	void ReceiveMessage(const DefendMessage& aMessage) override;

	const std::string& GetTargetName() const;

	static eComponentType GetType();
private:
	

	void FollowEntity(float aDeltaTime);
	void CalculateToTarget(eAITargetPositionMode aMode);

	float myTimeBetweenDecisions;
	std::string myTargetName;

	float myTimeToNextDecision;
	Entity* myEntityToFollow;
	Entity* myPrevEntityToFollow;
	CU::Vector3<float> myVelocity;
	CU::Vector3<float> myFollowingOffset;
	CU::Vector3<float> myToTarget;
	float myAvoidanceDistance;

	bool myIsEscaping;
	CU::Vector3<float> myEscapePosition;

	eAITargetPositionMode myTargetPositionMode;

	PhysicsComponent* myPhysicsComponent;
};

inline const std::string& AIComponent::GetTargetName() const
{
	return myTargetName;
}

inline eComponentType AIComponent::GetType()
{
	return eComponentType::CONTROLLER;
}