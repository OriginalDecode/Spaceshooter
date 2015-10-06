#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	AIComponent(Entity& aEntity);

	void Init();
	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity);

private:
	void MakeDecision();
	void FollowEntity(float aDeltaTime);
	void FollowOwnDecision(float aDeltaTime);

	float myTimeToNextDecision;
	int myDecision;
	
	Entity* myEntityToFollow;

	CU::Vector3<float> myVelocity;
	float mySpeed;
};

