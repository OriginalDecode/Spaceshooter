#include "stdafx.h"

#include <AudioInterface.h>
#include "ComponentEnums.h"
#include "Constants.h"
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <Engine.h>
#include "Entity.h"
#include "InputComponent.h"
#include "InputNote.h"
#include <InputWrapper.h>
#include <FileWatcher.h>
#include <XMLReader.h>
#include <sstream>
#include "SteeringTargetNote.h"

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

	WATCH_FILE("Data/script/player.xml", InputComponent::ReadXML);

	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Camera Lock", InputComponent::ToggleCameraLock, this);

	ReadXML("Data/script/player.xml");
}

void InputComponent::Update(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		myMovementSpeed += myAcceleration * aDeltaTime;
	}
	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		myMovementSpeed -= myAcceleration * aDeltaTime;
	}

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

	myMovementSpeed = CU::Clip(myMovementSpeed, myMinMovementSpeed, myMaxMovementSpeed);

	MoveForward(myMovementSpeed * aDeltaTime);


	Roll(aDeltaTime);

	if (myInputWrapper->MouseIsPressed(0) == true)
	{
		Shoot(myMovementSpeed);
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Laser");
	}

	if (myCameraIsLocked == false)
	{
		mySteering.x += CU::Clip(myInputWrapper->GetMouseDX(), -mySteeringDeltaClip, mySteeringDeltaClip);
		mySteering.y += CU::Clip(myInputWrapper->GetMouseDY(), -mySteeringDeltaClip, mySteeringDeltaClip);
	}

	if (mySteering.x > mySteeringDeaccelerationLowerLimit)
	{
		mySteering.x -= mySteeringDeacceleration * fabs(mySteering.x) * aDeltaTime;
		if (mySteering.x < 0.f)
		{
			mySteering.x = 0.f;
		}
	}
	else if (mySteering.x < -mySteeringDeaccelerationLowerLimit)
	{
		mySteering.x += mySteeringDeacceleration * fabs(mySteering.x) * aDeltaTime;
		if (mySteering.x > 0.f)
		{
			mySteering.x = 0.f;
		}
	}

	if (mySteering.y > mySteeringDeaccelerationLowerLimit)
	{
		mySteering.y -= mySteeringDeacceleration * fabs(mySteering.y) * aDeltaTime;
		if (mySteering.y < 0.f)
		{
			mySteering.y = 0.f;
		}
	}
	else if (mySteering.y < -mySteeringDeaccelerationLowerLimit)
	{
		mySteering.y += mySteeringDeacceleration * fabs(mySteering.y) * aDeltaTime;
		if (mySteering.y > 0.f)
		{
			mySteering.y = 0.f;
		}
	}

	//std::string tempX = std::to_string(mySteering.x);
	//tempX += ", " + std::to_string(mySteering.y);
	//SetWindowTextA(GetActiveWindow(), tempX.c_str());

	float xRotation = mySteering.x * aDeltaTime * mySteeringModifier;
	float yRotation = mySteering.y * aDeltaTime * mySteeringModifier;



	RotateX(yRotation);
	RotateY(xRotation);

	myEntity.SendNote<SteeringTargetNote>(SteeringTargetNote(mySteering));
}

void InputComponent::ReadXML(const std::string& aFile)
{
	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFile);
	reader.ForceReadAttribute(reader.FindFirstChild("movement"), "acceleration", myAcceleration);
	reader.ForceReadAttribute(reader.FindFirstChild("movement"), "maxMovementSpeed", myMaxMovementSpeed);
	reader.ForceReadAttribute(reader.FindFirstChild("movement"), "minMovementSpeed", myMinMovementSpeed);
	reader.ForceReadAttribute(reader.FindFirstChild("steering"), "modifier", mySteeringModifier);
	reader.ForceReadAttribute(reader.FindFirstChild("steering"), "deltaClip", mySteeringDeltaClip);
	reader.ForceReadAttribute(reader.FindFirstChild("steering"), "deacceleration", mySteeringDeacceleration);
	reader.ForceReadAttribute(reader.FindFirstChild("steering"), "deaccelerationLowerLimit", mySteeringDeaccelerationLowerLimit);
	reader.ForceReadAttribute(reader.FindFirstChild("steering"), "maxSteeringSpeed", myMaxSteeringSpeed);
	reader.ForceReadAttribute(reader.FindFirstChild("roll"), "acceleration", myRollAcceleration);
	reader.ForceReadAttribute(reader.FindFirstChild("roll"), "deacceleration", myRollDeacceleration);
	reader.ForceReadAttribute(reader.FindFirstChild("roll"), "maxRollSpeed", myMaxRollSpeed);
}

void InputComponent::Roll(float aDeltaTime)
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