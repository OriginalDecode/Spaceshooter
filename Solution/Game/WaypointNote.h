#pragma once
#include <Vector.h>

struct WaypointNote
{
	WaypointNote(const CU::Vector3<float>& aPosition);

	CU::Vector3<float> myPosition;
};

inline WaypointNote::WaypointNote(const CU::Vector3<float>& aPosition)
	: myPosition(aPosition)
{
}