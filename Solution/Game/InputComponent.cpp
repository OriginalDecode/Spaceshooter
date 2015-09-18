#include "stdafx.h"

#include "ComponentEnums.h"
#include "Constants.h"
#include "Entity.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include <XMLReader.h>


InputComponent::InputComponent()
{
}

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;

	XMLReader reader;
	reader.OpenDocument("Data/script/player.xml");
	reader.ReadAttribute(reader.FindFirstChild("steering"), "modifier", mySteeringModifier);
}

void InputComponent::Update(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		MoveForward(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		MoveBackward(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_A))
	{
		MoveLeft(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_D))
	{
		MoveRight(300.f * aDeltaTime);
	}

	if (myInputWrapper->KeyIsPressed(DIK_Q))
	{
		RotateZ(globalPi / 4.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_E))
	{
		RotateZ(-(globalPi / 4.f * aDeltaTime));
	}

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

	RotateX(yRotation);
	RotateY(xRotation);
}