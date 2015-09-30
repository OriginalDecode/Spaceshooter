#include "stdafx.h"

#include <AudioInterface.h>
#include "ComponentEnums.h"
#include "Constants.h"
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <Engine.h>
#include "Entity.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include <FileWatcher.h>
#include <XMLReader.h>
#include <sstream>
#include <Macros.h>

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;

	
	myRotationSpeed = 0.f;
	myMovementSpeed = 0.f;
	myMaxSteeringSpeed = 0;
	myCameraIsLocked = false;

	WATCH_FILE("Data/script/player.xml", InputComponent::ReadXML);

	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Camera Lock", InputComponent::ToggleCameraLock, this);

	ReadXML("Data/script/player.xml");
}

float clip(float n, float lower, float upper) 
{
	return MAX(lower, MIN(n, upper));
}

void InputComponent::Update(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		//myMovementSpeed = 50.f;
		MoveForward(50.f * aDeltaTime);
		
	}
	/*else
	{
		myMovementSpeed -= (globalPi * 5) * aDeltaTime;
		if (myMovementSpeed <= 0.f)
		{
			myMovementSpeed = 0.f;
		}
	}*/
	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		MoveBackward(50.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_A))
	{
		MoveLeft(50.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_D))
	{
		MoveRight(50.f * aDeltaTime);
	}

	Rotate(aDeltaTime);

	if (myInputWrapper->MouseIsPressed(0) == true)
	{
		Shoot(30000.f * aDeltaTime);
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Laser");
	}

	if (myCameraIsLocked == false)
	{
		myCursorPosition.x += static_cast<float>(clip(static_cast<float>(myInputWrapper->GetMouseDX()), -20.f, 20.f)) * aDeltaTime;
		myCursorPosition.y += static_cast<float>(clip(static_cast<float>(myInputWrapper->GetMouseDY()), -20.f, 20.f)) * aDeltaTime;
	}
	

	myCursorPosition.x = clip(myCursorPosition.x, -1, 1);
	myCursorPosition.y = clip(myCursorPosition.y, -1, 1);

	std::string tempX = std::to_string(myCursorPosition.x);
	SetWindowTextA(GetActiveWindow(), tempX.c_str());


	float negateX = myCursorPosition.x > 0.0f ? 1.0f : -1.0f;
	float negateY = myCursorPosition.y > 0.0f ? 1.0f : -1.0f;

	float x = myCursorPosition.x;
	float y = myCursorPosition.y;


	float xRotation = (fabs((x + (x*0.1f) ) * mySteeringModifier)  * negateX) * aDeltaTime;
	float yRotation = (fabs((y + (y*0.1f)) * mySteeringModifier)  * negateY) * aDeltaTime;

	if (xRotation > myMaxSteeringSpeed)
	{
		xRotation = myMaxSteeringSpeed;
	}
	if (xRotation < -myMaxSteeringSpeed)
	{
		xRotation = -myMaxSteeringSpeed;
	}

	if (yRotation > myMaxSteeringSpeed)
	{
		yRotation = myMaxSteeringSpeed;
	}
	if (yRotation < -myMaxSteeringSpeed)
	{
		yRotation = -myMaxSteeringSpeed;
	}


	RotateX(yRotation);
	RotateY(xRotation);
}

void InputComponent::ReadXML(const std::string& aFile)
{
	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFile);
	reader.ReadAttribute(reader.FindFirstChild("steering"), "modifier", mySteeringModifier);
	reader.ReadAttribute(reader.FindFirstChild("steering"), "maxSteeringSpeed", myMaxSteeringSpeed);
}

void InputComponent::Rotate(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_Q))
	{
		myRotationSpeed += (globalPi * 10) * aDeltaTime;
	}
	if (myInputWrapper->KeyIsPressed(DIK_E))
	{
		myRotationSpeed -= (globalPi * 10) * aDeltaTime;
	}

	if (myRotationSpeed > globalPi * 2)
	{
		myRotationSpeed = globalPi * 2;
	}
	if (myRotationSpeed < -(globalPi)* 2)
	{
		myRotationSpeed = -(globalPi)* 2;
	}

	if (myRotationSpeed > 0.f)
	{
		myRotationSpeed -= (globalPi * 5) * aDeltaTime;
		if (myRotationSpeed < 0.f)
		{
			myRotationSpeed = 0.f;
		}
	}
	else if (myRotationSpeed < 0.f)
	{
		myRotationSpeed += (globalPi * 5) * aDeltaTime;
		if (myRotationSpeed > 0.f)
		{
			myRotationSpeed = 0.f;
		}
	}

	RotateZ(myRotationSpeed * aDeltaTime);
}

void InputComponent::ToggleCameraLock()
{
	myCameraIsLocked = !myCameraIsLocked;
	myCursorPosition.x = 0.f;
	myCursorPosition.y = 0.f;
}