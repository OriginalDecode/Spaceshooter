#pragma once
#include <Vector.h>

class WaypointMessage
{
public:
	WaypointMessage(const CU::Vector3<float>& aPosition);

	const CU::Vector3<float>& GetPosition() const;

private:
	CU::Vector3<float> myPosition;
};

inline WaypointMessage::WaypointMessage(const CU::Vector3<float>& aPosition)
	: myPosition(aPosition)
{
}

inline const CU::Vector3<float>& WaypointMessage::GetPosition() const
{
	return myPosition;
}