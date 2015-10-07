#pragma once
#include "ControllerComponent.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class InputComponent : public ControllerComponent
{
public:
	InputComponent(Entity& aEntity);

	void Init(CU::InputWrapper& aInputWrapper);
	void Update(float aDeltaTime) override;

	static int GetID();
private:
	void operator=(const InputComponent&) = delete;
	void ReadXML(const std::string& aFile);

	void Roll(float aDeltaTime);
	void ToggleCameraLock();


	CU::InputWrapper* myInputWrapper;


	CU::Vector2<float> mySteering;
	float mySteeringModifier;
	float mySteeringDeltaClip;
	float mySteeringDeacceleration;
	float mySteeringDeaccelerationLowerLimit;
	float myRollSpeed;
	float myMaxRollSpeed;
	float myRollAcceleration;
	float myRollDeacceleration;
	float myMovementSpeed;
	float myMaxMovementSpeed;
	float myMinMovementSpeed;
	float myMaxSteeringSpeed;
	float myAcceleration;

	bool myCameraIsLocked;
};


inline int InputComponent::GetID()
{
	return 14;
}