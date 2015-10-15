#pragma once
#include "Enums.h"


struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);

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