#pragma once
#include "Message.h"

class PowerUpMessage : public Message
{
public:
	PowerUpMessage(CU::Vector3<float> aPos, float aRadius, float aTime);

	CU::Vector3<float> GetPosition() const;
	float GetRadius() const;
	float GetTime() const;

private:
	CU::Vector3<float> myPosition;
	float myRadius;

	float myDamage;
	float myTime;
};

inline CU::Vector3<float> PowerUpMessage::GetPosition() const
{
	return myPosition;
}

inline float PowerUpMessage::GetRadius() const
{
	return myRadius;
}

inline float PowerUpMessage::GetTime() const
{
	return myTime;
}