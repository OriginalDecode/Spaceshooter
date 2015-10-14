#pragma once

#include "Message.h"

class SpawnEnemyMessage : public Message
{
public:
	SpawnEnemyMessage(const CU::Vector3<float>& aPosition, const CU::Vector3<float>& aRotation
		, const CU::Vector3<float>& aScale);

private:
	const CU::Vector3<float> myPosition;
	const CU::Vector3<float> myRotation;
	const CU::Vector3<float> myScale;

};

