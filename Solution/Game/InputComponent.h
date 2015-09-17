#pragma once
#include "Component.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class InputComponent : public Component
{
public:
	InputComponent();

	void Init(CU::InputWrapper& aInputWrapper);
	void Update(float aDeltaTime) override;

	static int GetID();

private:
	void operator=(const InputComponent&) = delete;
	CU::InputWrapper* myInputWrapper;
};

inline int InputComponent::GetID()
{
	return 1;
}

