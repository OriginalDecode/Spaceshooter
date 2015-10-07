#pragma once
#include "Component.h"

#include <Matrix44.h>
class ControllerComponent : public Component
{
public:
	ControllerComponent(Entity& aEntity);

	void MoveUp(float aDistance);
	void MoveDown(float aDistance);
	void MoveLeft(float aDistance);
	void MoveRight(float aDistance);
	void MoveForward(float aDistance);
	void MoveBackward(float aDistance);
	void Move(const CU::Vector3<float>& aDirection);
	void RotateX(float aAmount);
	void RotateY(float aAmount);
	void RotateZ(float aAmount);
	void Rotate(const CU::Matrix44<float>& aRotation);
	void SetRotation(const CU::Matrix44<float>& aRotation);
	void Shoot(const float& aEnititySpeed = 0.f);

	static int GetID();
};

inline int ControllerComponent::GetID()
{
	return 1;
}