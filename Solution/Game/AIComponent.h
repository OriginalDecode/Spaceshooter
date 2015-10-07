#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	AIComponent(Entity& aEntity);

	void Init(float aSpeed, float aTimeBetweenDecisions);
	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity);

private:
	void FollowEntity(float aDeltaTime);

	float myTimeBetweenDecisions;
	float mySpeed;

	float myTimeToNextDecision;
	Entity* myEntityToFollow;
	CU::Vector3<float> myVelocity;
};

