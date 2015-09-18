#pragma once
#include "Component.h"
class ControllerComponent : public Component
{
public:
	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void MoveForward();
	virtual void MoveBackward();

	static int GetID();
};

inline int ControllerComponent::GetID()
{
	return 2;
}