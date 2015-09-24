#pragma once
#include "Component.h"
class CollisionComponent : public Component
{
public:
	void Init() override;
	void Update(float aDeltaTime) override;
	void ReceiveMessage(const RefreshOrientationMessage& aMessage) override;
	void ReceiveMessage(const ShootMessage& aMessage) override;

	static int GetID();

private:

	float myRadius;
	CU::Vector3<float> myPosition;


};

inline int CollisionComponent::GetID()
{
	return 7;
}