#pragma once

#include "Component.h"

class Entity;

class ShootingComponent : public Component
{
public:

	ShootingComponent();

	void ReceiveMessage(const ShootMessage& aMessage) override;
	void Init(CU::Vector3<float> aSpawningPointOffset = { 0.f, 0.f, 0.f });

	static int GetID();

private:

	CU::Vector3<float> mySpawningPointOffset;
};

inline int ShootingComponent::GetID()
{
	return 4;
}

