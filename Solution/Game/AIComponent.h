#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	void Init();
	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity);
private:

	struct SteeringOutput
	{
		CU::Vector3<float> myLinear;
		CU::Vector3<float> myAngular;
	};

	void MakeDecision();

	void FollowEntity(float aDeltaTime);
	void GetNewOrientation(const CU::Vector3<float>& aVelocity);
	SteeringOutput CalcualteSteering();

	void FollowOwnDecision(float aDeltaTime);



	float myTimeToNextDecision;
	int myDecision;
	
	Entity* myEntityToFollow;

	CU::Vector3<float> myOrientation;
	CU::Vector3<float> myAcceleration;
	CU::Vector3<float> myVelocity;

	float myMaxSpeed;
	float myMaxAcceleration;
	float myTargetRadius;
	float mySlowRadius;
	float myTimeToTarget;
};

