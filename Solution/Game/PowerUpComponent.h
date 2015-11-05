#pragma once
#include "Component.h"
#include "Enums.h"
class PowerUpComponent : public Component
{
public:
	PowerUpComponent(Entity& aEntity);
	void Init(ePowerUpType someType, std::string anInGameName, float someValue, float someDuration);
	void Init(ePowerUpType someType, std::string anInGameName, std::string aUpgradeName
		, std::string aPickupMessage, int anUpgradeID);

	void ReceiveNote(const CollisionNote& aNote) override;

	void Update(float aDeltaTime) override;
	void SetPlayer(Entity* aPlayer);

	static eComponentType GetType();

	std::string GetInGameName();

private:

	ePowerUpType myType;
	std::string myUpgradeName;
	std::string myUpgradePickupMessage;
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

inline std::string PowerUpComponent::GetInGameName()
{
	return myInGameName;
}