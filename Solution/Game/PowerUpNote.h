#pragma once
#include "Enums.h"

struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float aValue, float aDuration);
	PowerUpNote(ePowerUpType someType, std::string someUpgrade);

	const ePowerUpType myType;
	const std::string myUpgradeType;
	const float myDuration;
	const float myValue;

private:
	void operator=(PowerUpNote&) = delete;
};

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float aValue, float aDuration)
	: myType(someType)
	, myDuration(aDuration)
	, myValue(aValue)
	, myUpgradeType("")
{
}

inline PowerUpNote::PowerUpNote(ePowerUpType someType, std::string someUpgrade)
	: myType(someType)
	, myDuration(0.f)
	, myValue(0.f)
	, myUpgradeType(someUpgrade)
{
}