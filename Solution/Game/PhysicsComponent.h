#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Entity& aEntity);

	void Init(const CU::Matrix44<float>& anOrientation, const CU::Vector3<float>& aVelocity = { 0.f, 0.f, 0.f }, int aWeight = 1);
	void Init(int aWeight = 1, const CU::Vector3<float>& aVelocity = { 0.f, 0.f, 0.f });
	void Update(float aDeltaTime);

	static eComponentType GetType();

	int GetWeight() const;

	void SetVelocity(const CU::Vector3<float>& aVelocity);
	void AddVelocity(const CU::Vector3<float>& aVelocity);
	const CU::Vector3<float>& GetVelocity() const;

	void MoveForward(float aDistance);

	void Reset() override;
	//void BounceOff(float anEntityWeight);

private:
	CU::Vector3<float> myVelocity;
	int myWeight;
};

inline eComponentType PhysicsComponent::GetType()
{
	return eComponentType::PHYSICS;
}

inline int PhysicsComponent::GetWeight() const
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