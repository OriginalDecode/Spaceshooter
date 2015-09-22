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
	void FollowOwnDecision(float aDeltaTime);

	float myTimeToNextDecision;
	int myDecision;
	
	Entity* myEntityToFollow;

	float myRotX;
	float myRotY;
	float myRotZ;
};

