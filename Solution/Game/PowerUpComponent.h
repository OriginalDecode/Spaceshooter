#pragma once
#include "Component.h"
#include "Enums.h"
class PowerUpComponent : public Component
{
public:
	PowerUpComponent(Entity& aEntity);
	void Init(ePowerUpType someType, float someValue, float someDuration);
	void Init(ePowerUpType someType, std::string aUpgradeName, int anUpgradeID);

	void ReceiveNote(const CollisionNote& aNote) override;

	void Update(float aDeltaTime) override;
	void SetPlayer(Entity* aPlayer);

	static eComponentType GetType();
private:

	ePowerUpType myType;
	std::string myUpgradeName;
	std::string myInGameName;
	float myDuration;
	float myValue;
	int myUpgradeID;
	Entity* myPlayer;

};

inline eComponentType PowerUpComponent::GetType()
{
	return eComponentType::POWERUP;
}


