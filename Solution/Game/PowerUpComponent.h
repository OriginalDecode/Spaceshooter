#pragma once
#include "Component.h"
#include "Enums.h"
class PowerUpComponent : public Component
{
public:
	PowerUpComponent(Entity& aEntity);
	void Init(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);
	void Init(ePowerUpType someType, float someDuration);

	void ReceiveNote(const CollisionNote& aNote) override;

	void Update(float aDeltaTime) override;
	void SetPlayer(Entity* aPlayer);

	static eComponentType GetType();
private:

	ePowerUpType myType;
	float myDuration;
	int myShieldStrength;
	int myHealthRecover;
	int myFireRateMultiplier;
	Entity* myPlayer;

};

inline eComponentType PowerUpComponent::GetType()
{
	return eComponentType::POWERUP;
}


