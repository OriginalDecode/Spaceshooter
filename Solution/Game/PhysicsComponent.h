#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	void Init(const CU::Vector3<float> aVelocity, const CU::Vector3<float> aPosition);
	void Update(float aDeltaTime);
	void ReceiveMessage(eMessage aMessage) override;

	inline const CU::Vector3<float>& GetPosition();
	inline static int GetID();

private:
	CU::Vector3<float> myVelocity;
	CU::Vector3<float> myDirection;
	CU::Vector3<float> myPosition;
};

inline const CU::Vector3<float>& PhysicsComponent::GetPosition()
{
	return myPosition;
}

inline int PhysicsComponent::GetID()
{
	return 3;
}