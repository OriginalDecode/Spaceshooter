#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	void Init();
	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity);
private:
	void MakeDecision();

	void FollowEntity(float aDeltaTime);
	void GetNewOrientation(const CU::Vector3<float>& aVelocity);

	void FollowOwnDecision(float aDeltaTime);


	float myTimeToNextDecision;
	int myDecision;
	
	Entity* myEntityToFollow;

	CU::Vector3<float> myOrientation;
};

