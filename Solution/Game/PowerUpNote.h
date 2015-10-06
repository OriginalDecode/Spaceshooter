#pragma once

enum class ePowerUpType
{
	FIRERATEBOOST,
	SHIELDBOOST,
	HEALTHKIT_01,
	HEALTHKIT_02,

};

class PowerUpNote
{
public:

	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);

	const ePowerUpType GetType();
	const int GetShieldStrength();
	const int GetFireRateMultiplier();
	const int GetHealthRecover();
	const float GetDuration();

private:

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

inline const ePowerUpType  PowerUpNote::GetType()
{
	return myType;
}

inline const int  PowerUpNote::GetShieldStrength()
{
	return myShieldStrength;
}

inline const int  PowerUpNote::GetFireRateMultiplier()
{
	return myFireRateMultiplier;
}

inline const int  PowerUpNote::GetHealthRecover()
{
	return myHealthRecover;
}

inline const float  PowerUpNote::GetDuration()
{
	return myDuration;
}