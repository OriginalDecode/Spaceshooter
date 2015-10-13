#pragma once
#include "ControllerComponent.h"

class AIComponent : public ControllerComponent
{
public:
	AIComponent(Entity& aEntity);

	void Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName);
	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity);

	const std::string& GetTargetName() const;

	static eComponentType GetType();
private:
	void FollowEntity(float aDeltaTime);

	float myTimeBetweenDecisions;
	std::string myTargetName;

	float myTimeToNextDecision;
	Entity* myEntityToFollow;
	bool myFollowingEntity;
	CU::Vector3<float> myTargetPosition;
	CU::Vector3<float> myVelocity;
};

inline const std::string& AIComponent::GetTargetName() const
{
	return myTargetName;
}

inline eComponentType AIComponent::GetType()
{
	return eComponentType::AI;
}