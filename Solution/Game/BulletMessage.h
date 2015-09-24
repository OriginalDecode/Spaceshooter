#pragma once
#include "BulletEnums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(const eBulletType& aType, const CU::Vector3<float>& aVelocity, const CU::Matrix44<float>& anOrientation);

	const eBulletType& GetBulletType() const;
	const CU::Vector3<float>& GetVelocity() const;
	const CU::Matrix44<float>& GetOrientation() const;

private:

	CU::Matrix44<float> myOrientation;
	CU::Vector3<float> myVelocity;
	eBulletType myType;
};

inline const eBulletType& BulletMessage::GetBulletType() const
{
	return myType;
}

inline const CU::Vector3<float>& BulletMessage::GetVelocity() const
{
	return myVelocity;
}

inline const CU::Matrix44<float>& BulletMessage::GetOrientation() const
{
	return myOrientation;
}