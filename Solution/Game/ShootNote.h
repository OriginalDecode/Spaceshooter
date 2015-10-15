#pragma once

struct ShootNote
{
	ShootNote(const CU::Vector3<float>& aVelocity);

	const CU::Vector3<float> myEnitityVelocity;
};

inline ShootNote::ShootNote(const CU::Vector3<float>& aVelocity)
	: myEnitityVelocity(aVelocity)
{
}