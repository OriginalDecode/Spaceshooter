#pragma once
#include "BulletEnums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(eBulletType aType, CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition);
	~BulletMessage();

	inline const eBulletType& GetBulletType() const;
	inline const CU::Vector3<float>& GetVelocity() const;
	inline const CU::Vector3<float>& GetPosition() const;

private:

	eBulletType myType;

	CU::Vector3<float> myVelocity;
	CU::Vector3<float> myPosition;
};

inline const eBulletType& BulletMessage::GetBulletType() const
{
	return myType;
}

inline const CU::Vector3<float>& BulletMessage::GetVelocity() const
{
	return myVelocity;
}

inline const CU::Vector3<float>& BulletMessage::GetPosition() const
{
	return myPosition;
}