#pragma once
#include <Vector.h>

class SteeringTargetNote
{
public:
	SteeringTargetNote(const CU::Vector2<float>& aPosition);
	
	const CU::Vector2<float> GetPosition() const;

private:
	CU::Vector2<float> myPosition;
};

inline SteeringTargetNote::SteeringTargetNote(const CU::Vector2<float>& aPosition)
	: myPosition(aPosition)
{
}

inline const CU::Vector2<float> SteeringTargetNote::GetPosition() const
{
	return myPosition;
}