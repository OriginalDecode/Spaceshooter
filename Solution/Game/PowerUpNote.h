#pragma once

enum class ePowerUpType
{
	FIRERATEBOOST,
	SHIELDBOOST,
	HEALTHKIT_01,
	HEALTHKIT_02,

};

struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);

	ePowerUpType myType;
	float myDuration;
	int myShieldStrength;
	int myHealthRecover;
	int myFireRateMultiplier;

};

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(someShieldStrength)
	, myHealthRecover(someHealthRecover)
	, myFireRateMultiplier(myFireRateMultiplier)
{
}