#pragma once

struct ShootNote
{
	ShootNote(const float& aEnititySpeed);

	float myEnititySpeed;
};

inline ShootNote::ShootNote(const float& aEnititySpeed)
{
	myEnititySpeed = aEnititySpeed;
}