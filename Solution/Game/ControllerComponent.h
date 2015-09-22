#pragma once
#include "Component.h"

#include <Matrix44.h>
class ControllerComponent : public Component
{
public:
	void MoveUp(float aDistance);
	void MoveDown(float aDistance);
	void MoveLeft(float aDistance);
	void MoveRight(float aDistance);
	void MoveForward(float aDistance);
	void MoveBackward(float aDistance);
	void RotateX(float aAmount);
	void RotateY(float aAmount);
	void RotateZ(float aAmount);
	void Rotate(const CU::Matrix44<float>& aRotation);
	void Shoot(float aSpeed);

	static int GetID();
};

inline int ControllerComponent::GetID()
{
	return 1;
}