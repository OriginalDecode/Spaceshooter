#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();

	void Init(const CU::Vector3<float> aVelocity, const CU::Matrix44<float>& anOrientation);
	void Update(float aDeltaTime);

	static int GetID();

private:
	CU::Vector3<float> myVelocity;
};

inline int PhysicsComponent::GetID()
{
	return 3;
}