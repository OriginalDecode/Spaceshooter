#pragma once

struct ShootNote
{
	ShootNote(const CU::Vector3<float>& aVelocity);

	const CU::Vector3<float> myEnitityVelocity;

private:
	void operator=(ShootNote&) = delete;
};

inline ShootNote::ShootNote(const CU::Vector3<float>& aVelocity)
	: myEnitityVelocity(aVelocity)
{
}