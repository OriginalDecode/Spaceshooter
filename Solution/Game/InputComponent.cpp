#include "stdafx.h"

#include "ComponentEnums.h"
#include "Constants.h"
#include <Engine.h>
#include "Entity.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include <FileWatcher.h>
#include <XMLReader.h>


InputComponent::InputComponent()
{
}

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;

	
	myRotationSpeed = 0.f;
	myMovementSpeed = 0.f;
	myMaxSteeringSpeed = 0;


	//Prism::Engine::GetInstance()->GetFileWatcher().WatchFile("Data/script/player.xml", std::bind(&InputComponent::ReadXML, this, "Data/script/player.xml"));
	WATCH_FILE("Data/script/player.xml", InputComponent::ReadXML);

	ReadXML("Data/script/player.xml");
}

void InputComponent::Update(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		myMovementSpeed = 50.f;
	}
	else
	{
		myMovementSpeed -= globalPi / 128.f;
		if (myMovementSpeed <= 0.f)
		{
			myMovementSpeed = 0.f;
		}
	}
	MoveForward(myMovementSpeed * aDeltaTime);
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

	if (myInputWrapper->MouseDown(0))
	{
		Shoot(30000.f * aDeltaTime);
	}

	myCursorPosition.x += static_cast<float>(myInputWrapper->GetMouseDX()) * 0.001f;
	myCursorPosition.y += static_cast<float>(myInputWrapper->GetMouseDY()) * 0.001f;

	float negateX = myCursorPosition.x > 0.0f ? 1.0f : -1.0f;
	float negateY = myCursorPosition.y > 0.0f ? 1.0f : -1.0f;

	float x = myCursorPosition.x;
	float y = myCursorPosition.y;

	if (x < 0.001f && x > -0.001f)
	{
		x = 0;
	}
	if (y < 0.001f && y > -0.001f)
	{
		y = 0;
	}


	float xRotation = (fabs((x*x) * mySteeringModifier)  * negateX) * aDeltaTime;
	float yRotation = (fabs((y*y) * mySteeringModifier)  * negateY) * aDeltaTime;

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
		myRotationSpeed += globalPi / 128.f;
	}
	if (myInputWrapper->KeyIsPressed(DIK_E))
	{
		myRotationSpeed -= globalPi / 128.f;
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
		myRotationSpeed -= globalPi / 256.f;
		if (myRotationSpeed < 0.f)
		{
			myRotationSpeed = 0.f;
		}
	}
	else if (myRotationSpeed < 0.f)
	{
		myRotationSpeed += globalPi / 256.f;
		if (myRotationSpeed > 0.f)
		{
			myRotationSpeed = 0.f;
		}
	}

	RotateZ(myRotationSpeed * aDeltaTime);
}