#pragma once
#include "Component.h"

class PowerUpComponent : public Component
{
public:
	PowerUpComponent(Entity& aEntity);
	void ReceiveNote(const PowerUpNote& aNote) override;
	void ReceiveNote(const CollisionNote& aNote) override;

	static int GetID();


};

inline int PowerUpComponent::GetID()
{
	return 12;
}


