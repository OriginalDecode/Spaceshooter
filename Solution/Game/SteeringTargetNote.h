#pragma once
#include <Vector.h>

struct SteeringTargetNote
{
	SteeringTargetNote(const CU::Vector2<float>& aPosition);
	
	CU::Vector2<float> myPosition;
};

inline SteeringTargetNote::SteeringTargetNote(const CU::Vector2<float>& aPosition)
	: myPosition(aPosition)
{
}