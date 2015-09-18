#pragma once
#include "ControllerComponent.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class InputComponent : public ControllerComponent
{
public:
	InputComponent();

	void Init(CU::InputWrapper& aInputWrapper);
	void Update(float aDeltaTime) override;

private:
	void operator=(const InputComponent&) = delete;
	void Rotate(float aDeltaTime);
	CU::InputWrapper* myInputWrapper;


	CU::Vector2<float> myCursorPosition;
	float mySteeringModifier;
	float myRotationSpeed;
	float myMovementSpeed;
};
