#include "stdafx.h"

#include <AudioInterface.h>
#include "Constants.h"
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <Engine.h>
#include "Entity.h"
#include <FileWatcher.h>
#include "GUINote.h"
#include "InputComponent.h"
#include "InputNote.h"
#include <InputWrapper.h>
#include "PhysicsComponent.h"
#include <sstream>
#include <XMLReader.h>

InputComponent::InputComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
{

}

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;

	myRollSpeed = 0.f;
	myMovementSpeed = 0.f;
	myMaxSteeringSpeed = 0;
	myMaxRollSpeed = 0;
	myCameraIsLocked = false;
	myBoost = false;
	myCanMove = true;
	myWeaponRotationModifier = 0.f;
	myCurrentBoostCooldown = 0.f;
	myCurrentBoostValue = 0.f;
	myMaxBoostCooldown = 0.f;
	myMaxBoostValue = 0.f;

	WATCH_FILE("Data/Setting/SET_player.xml", InputComponent::ReadXML);

	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Camera Lock", InputComponent::ToggleCameraLock, this);

	ReadXML("Data/Setting/SET_player.xml");
}

void InputComponent::Update(float aDeltaTime)
{
	myBoost = false;

	if (myCanMove == true)
	{
		if (myInputWrapper->KeyIsPressed(DIK_1))
		{
			myEntity.SendNote(InputNote(0));
		}
		if (myInputWrapper->KeyIsPressed(DIK_2))
		{
			myEntity.SendNote(InputNote(1));
		}
		if (myInputWrapper->KeyIsPressed(DIK_3))
		{
			myEntity.SendNote(InputNote(2));
		}
		if (myInputWrapper->MouseIsPressed(0) == true)
		{
			Shoot(myEntity.GetComponent<PhysicsComponent>()->GetVelocity(), mySteering * mySteeringModifier * myWeaponRotationModifier);
		}

		if (myInputWrapper->KeyIsPressed(DIK_LSHIFT) || myInputWrapper->KeyIsPressed(DIK_RSHIFT))
		{
			myBoost = true;
		}

		UpdateMovement(aDeltaTime);
		UpdateSteering(aDeltaTime);		
	}
	else
	{
		myMovementSpeed = 0.f;

		myTimeBeforeMovement -= aDeltaTime;

		if (myTimeBeforeMovement <= 0.f)
		{
			myCanMove = true;
		}

		RotateX(aDeltaTime / 80);
		RotateZ(aDeltaTime / 70);
	}

	Roll(aDeltaTime);

	float xRotation = mySteering.x * aDeltaTime * mySteeringModifier;
	float yRotation = mySteering.y * aDeltaTime * mySteeringModifier;

	RotateX(yRotation);
	RotateY(xRotation);

	myEntity.SendNote<GUINote>(GUINote({ mySteering.x, mySteering.y, 0 }, eGUINoteType::STEERING_TARGET));
}

void InputComponent::ReadXML(const std::string& aFile)
{
	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFile);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("movement"), "acceleration", myAcceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("movement"), "maxMovementSpeed", myMaxMovementSpeed);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("movement"), "minMovementSpeed", myMinMovementSpeed);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("steering"), "modifier", mySteeringModifier);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("steering"), "deltaClip", mySteeringDeltaClip);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("steering"), "deacceleration", mySteeringDeacceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("steering"), "deaccelerationLowerLimit", mySteeringDeaccelerationLowerLimit);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("steering"), "maxSteeringSpeed", myMaxSteeringSpeed);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("roll"), "acceleration", myRollAcceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("roll"), "deacceleration", myRollDeacceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("roll"), "maxRollSpeed", myMaxRollSpeed);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("boost"), "acceleration", myBoostAcceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("boost"), "deacceleration", myBoostDeacceleration);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("boost"), "maxBoost", myMaxBoostValue);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("boost"), "cooldown", myMaxBoostCooldown);
	reader.ForceReadAttribute(reader.ForceFindFirstChild("weaponRotation"), "modifier", myWeaponRotationModifier);
	reader.CloseDocument();
}

void InputComponent::Roll(float aDeltaTime)
{
	if (myCanMove == true)
	{
		if (myInputWrapper->KeyIsPressed(DIK_Q) || myInputWrapper->KeyIsPressed(DIK_A))
		{
			myRollSpeed += myRollAcceleration * aDeltaTime;
		}
		if (myInputWrapper->KeyIsPressed(DIK_E) || myInputWrapper->KeyIsPressed(DIK_D))
		{
			myRollSpeed -= myRollAcceleration * aDeltaTime;
		}

		myRollSpeed = CU::Clip(myRollSpeed, -myMaxRollSpeed, myMaxRollSpeed);
	}

	if (myRollSpeed > 0.f)
	{
		myRollSpeed -= myRollDeacceleration * aDeltaTime;
		if (myRollSpeed < 0.f)
		{
			myRollSpeed = 0.f;
		}
	}
	else if (myRollSpeed < 0.f)
	{
		myRollSpeed += myRollDeacceleration * aDeltaTime;
		if (myRollSpeed > 0.f)
		{
			myRollSpeed = 0.f;
		}
	}

	RotateZ(myRollSpeed * aDeltaTime);
}

void InputComponent::ToggleCameraLock()
{
	myCameraIsLocked = !myCameraIsLocked;
	mySteering.x = 0.f;
	mySteering.y = 0.f;
}

void InputComponent::UpdateMovement(const float& aDelta)
{
	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		myMovementSpeed += myAcceleration * aDelta;
	}
	else
	{
		myMovementSpeed -= myAcceleration * aDelta / 10;
	}

	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		myMovementSpeed -= myAcceleration * aDelta;
	}

	int soundSpeed = static_cast<int>((myMovementSpeed / myMaxMovementSpeed) * 100);
	Prism::Audio::AudioInterface::GetInstance()->SetRTPC("SS_Air_RPM", soundSpeed, GetEntity().GetAudioSFXID());
	int boostSpeed = static_cast<int>((myCurrentBoostValue / myMaxBoostValue) * 100);
	Prism::Audio::AudioInterface::GetInstance()->SetRTPC("SS_Air_Storm", boostSpeed, GetEntity().GetAudioSFXID());

	myMovementSpeed = CU::Clip(myMovementSpeed, myMinMovementSpeed, myMaxMovementSpeed);
	myMovementSpeed += myCurrentBoostValue;

	if (myCurrentBoostCooldown == 0.f)
	{
		if (myBoost == true)
		{
			myCurrentBoostValue += aDelta * myBoostAcceleration;
			if (myCurrentBoostValue >= myMaxBoostValue)
			{
				myCurrentBoostValue = myMaxBoostValue;
				myCurrentBoostCooldown = myMaxBoostCooldown;
			}
		}
	}
	else
	{
		myCurrentBoostCooldown -= aDelta;
		if (myCurrentBoostCooldown <= 0.f)
		{
			myCurrentBoostCooldown = 0.f;
		}
	}
	
	myEntity.GetComponent<PhysicsComponent>()->MoveForward(myMovementSpeed);

	if (myBoost == false)
	{
		myCurrentBoostValue -= aDelta * myBoostDeacceleration;
		if (myCurrentBoostValue < 0.f)
		{
			myCurrentBoostValue = 0.f;
		}
	}
}

void InputComponent::UpdateSteering(const float& aDelta)
{
	if (myCameraIsLocked == false || myCanMove == false)
	{
		mySteering.x += CU::Clip(myInputWrapper->GetMouseDX(), -mySteeringDeltaClip, mySteeringDeltaClip);
		mySteering.y += CU::Clip(myInputWrapper->GetMouseDY(), -mySteeringDeltaClip, mySteeringDeltaClip);
	}

	if (mySteering.x > mySteeringDeaccelerationLowerLimit)
	{
		mySteering.x -= mySteeringDeacceleration * fabs(mySteering.x) * aDelta;
		if (mySteering.x < 0.f)
		{
			mySteering.x = 0.f;
		}
	}
	else if (mySteering.x < -mySteeringDeaccelerationLowerLimit)
	{
		mySteering.x += mySteeringDeacceleration * fabs(mySteering.x) * aDelta;
		if (mySteering.x > 0.f)
		{
			mySteering.x = 0.f;
		}
	}

	if (mySteering.y > mySteeringDeaccelerationLowerLimit)
	{
		mySteering.y -= mySteeringDeacceleration * fabs(mySteering.y) * aDelta;
		if (mySteering.y < 0.f)
		{
			mySteering.y = 0.f;
		}
	}
	else if (mySteering.y < -mySteeringDeaccelerationLowerLimit)
	{
		mySteering.y += mySteeringDeacceleration * fabs(mySteering.y) * aDelta;
		if (mySteering.y > 0.f)
		{
			mySteering.y = 0.f;
		}
	}
}

void InputComponent::Reset()
{
	myRollSpeed = 0.f;
	myMovementSpeed = 0.f;
	myMaxSteeringSpeed = 0.f;
	myMaxRollSpeed = 0.f;
	myCameraIsLocked = false;
	myBoost = false;
	myCanMove = true;
	myWeaponRotationModifier = 0.f;
	myCurrentBoostCooldown = 0.f;
	myCurrentBoostValue = 0.f;
	myMaxBoostCooldown = 0.f;
	myMaxBoostValue = 0.f;
}