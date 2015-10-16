#pragma once
#include "Enums.h"


struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);
	PowerUpNote(ePowerUpType someType, float someDuration);

	const ePowerUpType myType;
	const float myDuration;
	const int myShieldStrength;
	const int myHealthRecover;
	const int myFireRateMultiplier;

private:
	void operator=(PowerUpNote&) = delete;
};

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(someShieldStrength)
	, myHealthRecover(someHealthRecover)
	, myFireRateMultiplier(someFireRateMultiplier)
{
	DL_ASSERT_EXP(myHealthRecover >= 0, "Can't recover negative health.");
}

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(0)
	, myHealthRecover(0)
	, myFireRateMultiplier(0)
{
}