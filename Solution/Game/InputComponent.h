#pragma once
#include "ControllerComponent.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class InputComponent : public ControllerComponent
{
public:
	void Init(CU::InputWrapper& aInputWrapper);
	void Update(float aDeltaTime) override;

private:
	void operator=(const InputComponent&) = delete;
	void ReadXML(const std::string& aFile);

	void Rotate(float aDeltaTime);
	void ToggleCameraLock();


	CU::InputWrapper* myInputWrapper;


	CU::Vector2<float> myCursorPosition;
	float mySteeringModifier;
	float myRotationSpeed;
	float myMovementSpeed;
	float myMaxSteeringSpeed;
	bool myCameraIsLocked;
};
