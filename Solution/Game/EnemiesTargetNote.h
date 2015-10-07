#pragma once
#include <Vector.h>

struct EnemiesTargetNote
{
	EnemiesTargetNote(const CU::Vector3<float>& aPosition);

	CU::Vector3<float> myPosition;
};

inline EnemiesTargetNote::EnemiesTargetNote(const CU::Vector3<float>& aPosition)
	: myPosition(aPosition)
{
}