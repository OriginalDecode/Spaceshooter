#pragma once
#include <Vector.h>

class EnemiesTargetNote
{
public:
	EnemiesTargetNote(const CU::Vector3<float>& aPosition);

	const CU::Vector3<float> GetPosition() const;

private:
	CU::Vector3<float> myPosition;
};

inline EnemiesTargetNote::EnemiesTargetNote(const CU::Vector3<float>& aPosition)
	: myPosition(aPosition)
{
}

inline const CU::Vector3<float> EnemiesTargetNote::GetPosition() const
{
	return myPosition;
}