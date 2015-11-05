#pragma once

#include "ControllerComponent.h"
#include "Enums.h"
#include "Subscriber.h"

class BulletComponent;
class PhysicsComponent;

class BulletAIComponent : public ControllerComponent
{
public:
	BulletAIComponent(Entity& aEntity);
	~BulletAIComponent();

	void Init(float aSpeed, float aTurnRateModifier);

	void Update(float aDeltaTime) override;

	void SetEntityToFollow(Entity* aEntity, Entity* aPlayerEntity);

	const std::string& GetTargetName() const;
	static eComponentType GetType();
private:

	void FollowEntity(float aDeltaTime);

	Entity* myEntityToFollow;
	Entity* myPrevEntityToFollow;
	CU::Vector3<float> myVelocity;
	CU::Vector3<float> myToTarget;

	float myTurnRateModifier;

	PhysicsComponent* myPhysicsComponent;
	BulletComponent* myBulletComponent;
};

inline eComponentType BulletAIComponent::GetType()
{
	return eComponentType::BULLET_AI;
}