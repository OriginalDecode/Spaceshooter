#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Entity& aEntity);

	void Init(const CU::Matrix44<float>& anOrientation, const CU::Vector3<float> aVelocity = { 0.f, 0.f, 0.f }, const unsigned short aWeight = 1.f);
	void Init(const unsigned short aWeight = 1.f, const CU::Vector3<float> aVelocity = { 0.f, 0.f, 0.f });
	void Update(float aDeltaTime);

	static int GetID();

	const unsigned short& GetWeight() const;

	void SetVelocity(const CU::Vector3<float>& aVelocity);
	void AddVelocity(const CU::Vector3<float>& aVelocity);
	const CU::Vector3<float>& GetVelocity() const;

	void MoveForward(const float& aDistance);

	void BounceOff(const float& anEntityWeight);

private:
	CU::Vector3<float> myVelocity;
	unsigned short myWeight;
};

inline int PhysicsComponent::GetID()
{
	return 3;
}

inline const unsigned short& PhysicsComponent::GetWeight() const
{
	return myWeight;
}

inline void PhysicsComponent::SetVelocity(const CU::Vector3<float>& aVelocity)
{
	myVelocity = aVelocity;
}

inline void PhysicsComponent::AddVelocity(const CU::Vector3<float>& aVelocity)
{
	myVelocity += aVelocity;
}

inline const CU::Vector3<float>& PhysicsComponent::GetVelocity() const
{
	return myVelocity;
}