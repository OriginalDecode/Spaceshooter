#pragma once
#include "Enums.h"


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
	, myFireRateMultiplier(someFireRateMultiplier)
{
	DL_ASSERT_EXP(myHealthRecover >= 0, "Can't recover negative health.");
}