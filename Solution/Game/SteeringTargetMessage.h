#pragma once
#include <Vector.h>

class SteeringTargetMessage
{
public:
	SteeringTargetMessage(const CU::Vector2<float>& aPosition);
	
	const CU::Vector2<float> GetPosition() const;

private:
	CU::Vector2<float> myPosition;
};

inline SteeringTargetMessage::SteeringTargetMessage(const CU::Vector2<float>& aPosition)
	: myPosition(aPosition)
{
}

inline const CU::Vector2<float> SteeringTargetMessage::GetPosition() const
{
	return myPosition;
}