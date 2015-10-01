#pragma once
#include "Enums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(eBulletType aType, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType);

	const eBulletType& GetBulletType() const;
	const CU::Matrix44<float>& GetOrientation() const;

private:
	void operator=(BulletMessage&) = delete;
	CU::Matrix44<float> myOrientation;
	const eBulletType myType;
	const eEntityType myEntityType;
};

inline const eBulletType& BulletMessage::GetBulletType() const
{
	return myType;
}

inline const CU::Matrix44<float>& BulletMessage::GetOrientation() const
{
	return myOrientation;
}