#pragma once
#include "Component.h"

class InputComponent : public Component
{
public:
	InputComponent();

	void Init() override;
	void Update(float aDeltaTime) override;

	static int GetID();
};

inline int InputComponent::GetID()
{
	return 1;
}

