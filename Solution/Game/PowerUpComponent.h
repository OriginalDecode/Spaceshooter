#pragma once
#include "Component.h"
#include "Enums.h"
class PowerUpComponent : public Component
{
public:
	PowerUpComponent(Entity& aEntity);
	void Init(ePowerUpType someType);
	void ReceiveNote(const CollisionNote& aNote) override;

	static int GetID();
private:

	ePowerUpType myType;

};

inline int PowerUpComponent::GetID()
{
	return 12;
}


