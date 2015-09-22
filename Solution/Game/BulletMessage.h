#pragma once
#include "BulletEnums.h"
#include "Message.h"

class BulletMessage : public Message
{
public:
	BulletMessage(eBulletType aType, CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition, CU::Vector3<float> aForward);
	~BulletMessage();

	const eBulletType& GetBulletType() const;
	const CU::Vector3<float>& GetVelocity() const;
	const CU::Vector3<float>& GetPosition() const;
	const CU::Vector3<float>& GetForward() const;

private:

	eBulletType myType;

	CU::Vector3<float> myVelocity;
	CU::Vector3<float> myPosition;
	CU::Vector3<float> myForward;
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

inline const CU::Vector3<float>& BulletMessage::GetForward() const
{
	return myForward;
}