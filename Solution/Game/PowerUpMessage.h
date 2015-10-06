#pragma once

enum class ePowerUpType
{
	NO_POWERUP = -1,
	FIRERATEBOOST = 0,
	SHIELDBOOST,
	HEALTHKIT_01,
	HEALTHKIT_02,


};

class PowerUpMessage
{
public:

	PowerUpMessage(ePowerUpType someType, float someDuration, int someShieldStrength
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

inline PowerUpMessage::PowerUpMessage(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(someShieldStrength)
	, myHealthRecover(someHealthRecover)
	, myFireRateMultiplier(myFireRateMultiplier)
{
}

inline const ePowerUpType  PowerUpMessage::GetType()
{
	return myType;
}

inline const int  PowerUpMessage::GetShieldStrength()
{
	return myShieldStrength;
}

inline const int  PowerUpMessage::GetFireRateMultiplier()
{
	return myFireRateMultiplier;
}

inline const int  PowerUpMessage::GetHealthRecover()
{
	return myHealthRecover;
}

inline const float  PowerUpMessage::GetDuration()
{
	return myDuration;
}