#pragma once
#include "Enums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType, const float& aEnititySpeed);

	eBulletType GetBulletType() const;
	eEntityType GetEntityType() const;
	const CU::Matrix44<float>& GetOrientation() const;
	const float& GetSpeedMultiplier() const;

private:
	void operator=(BulletMessage&) = delete;
	CU::Matrix44<float> myOrientation;
	const eBulletType myType;
	const eEntityType myEntityType;
	const float mySpeedMultiplier;
};

inline eBulletType BulletMessage::GetBulletType() const
{
	return myType;
}

inline eEntityType BulletMessage::GetEntityType() const
{
	return myEntityType;
}

inline const CU::Matrix44<float>& BulletMessage::GetOrientation() const
{
	return myOrientation;
}

inline const float& BulletMessage::GetSpeedMultiplier() const
{
	return mySpeedMultiplier;
}