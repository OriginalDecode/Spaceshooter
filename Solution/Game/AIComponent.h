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
	void GetNewOrientation(const CU::Vector3<float>& aVelocity);
	void CalcualteSteering(float aDeltaTime);

	void FollowOwnDecision(float aDeltaTime);



	float myTimeToNextDecision;
	int myDecision;
	
	Entity* myEntityToFollow;

	//CU::Vector3<float> myOrientation;
	//CU::Vector3<float> myAcceleration;
	//CU::Vector3<float> myVelocity;

	float mySpeed;

	float myMaxSpeed;
	float myMaxAcceleration;
	float myTargetRadius;
	float mySlowRadius;
	float myTimeToTarget;
};

