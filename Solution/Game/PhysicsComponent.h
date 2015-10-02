#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Entity& aEntity);

	void Init(const CU::Matrix44<float>& anOrientation, const CU::Vector3<float> aVelocity = {0.f, 0.f, 0.f} );
	void Update(float aDeltaTime);

	static int GetID();

private:
	CU::Vector3<float> myVelocity;
};

inline int PhysicsComponent::GetID()
{
	return 3;
}