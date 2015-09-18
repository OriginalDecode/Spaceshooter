#pragma once

#include "Component.h"

class Entity;

class ShootingComponent : public Component
{
public:
	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveMessage(const ShootMessage& aMessage) override;
	virtual void ReceiveMessage(const RefreshOrientationMessage& aMessage) override;

	static int GetID();

private:
	Entity* myBullet;
	bool myIsShooting;
	CU::Vector3<float> myPosition;
	CU::Vector3<float> myForward;
};

inline int ShootingComponent::GetID()
{
	return 4;
}

