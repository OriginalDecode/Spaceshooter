#pragma once
#include "Enums.h"


struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, unsigned short someHealthRecover, int someFireRateMultiplier);

	ePowerUpType myType;
	float myDuration;
	int myShieldStrength;
	unsigned short myHealthRecover;
	int myFireRateMultiplier;

};

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, unsigned short someHealthRecover, int someFireRateMultiplier)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(someShieldStrength)
	, myHealthRecover(someHealthRecover)
	, myFireRateMultiplier(someFireRateMultiplier)
{
	DL_ASSERT_EXP(myHealthRecover >= 0, "Can't recover negative health.");
}