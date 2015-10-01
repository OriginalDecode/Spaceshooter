#pragma once
#include "Enums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType);

	eBulletType GetBulletType() const;
	eEntityType GetEntityType() const;
	const CU::Matrix44<float>& GetOrientation() const;

private:
	void operator=(BulletMessage&) = delete;
	CU::Matrix44<float> myOrientation;
	const eBulletType myType;
	const eEntityType myEntityType;
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