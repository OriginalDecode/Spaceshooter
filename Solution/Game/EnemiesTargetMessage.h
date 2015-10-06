#pragma once
#include <Vector.h>

class EnemiesTargetMessage
{
public:
	EnemiesTargetMessage(const CU::Vector3<float>& aPosition);

	const CU::Vector3<float> GetPosition() const;

private:
	CU::Vector3<float> myPosition;
};

inline EnemiesTargetMessage::EnemiesTargetMessage(const CU::Vector3<float>& aPosition)
	: myPosition(aPosition)
{
}

inline const CU::Vector3<float> EnemiesTargetMessage::GetPosition() const
{
	return myPosition;
}