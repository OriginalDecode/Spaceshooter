#pragma once
#include "Component.h"

#include <Matrix44.h>
class ControllerComponent : public Component
{
public:
	ControllerComponent(Entity& aEntity);
	virtual ~ControllerComponent();
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
	void Shoot(const CU::Vector3<float>& aVelocity = { 0.f, 0.f, 0.f }, const CU::Vector2<float>& aSteering = { 0.f, 0.f });

	void SetCanMove(const bool& aCanMove);
	void DisableMovement(const float& aSeconds);

	static eComponentType GetType();

protected:
	float myTimeBeforeMovement;
	bool myCanMove;
};

inline void ControllerComponent::SetCanMove(const bool& aCanMove)
{
	myCanMove = aCanMove;
}

inline eComponentType ControllerComponent::GetType()
{
	return eComponentType::CONTROLLER;
}