#pragma once

#include "Component.h"

class Entity;

class ShootingComponent : public Component
{
public:
	virtual void ReceiveMessage(const ShootMessage& aMessage) override;

	static int GetID();

private:
};

inline int ShootingComponent::GetID()
{
	return 4;
}

