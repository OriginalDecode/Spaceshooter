#pragma once
#include "Component.h"

class PowerUpComponent : public Component
{
public:

	void ReceiveMessage(const PowerUpMessage& aMessage) override;

	static int GetID();


};

inline int PowerUpComponent::GetID()
{
	return 12;
}


