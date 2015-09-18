#include "stdafx.h"

#include "ComponentEnums.h"
#include "Constants.h"
#include "Entity.h"
#include "InputComponent.h"
#include <InputWrapper.h>


InputComponent::InputComponent()
{
}

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;
}

void InputComponent::Update(float aDeltaTime)
{
	if (myInputWrapper->KeyIsPressed(DIK_U))
	{
		MoveForward(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_J))
	{
		MoveBackward(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_H))
	{
		MoveLeft(300.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_K))
	{
		MoveRight(300.f * aDeltaTime);
	}

	if (myInputWrapper->KeyIsPressed(DIK_Y))
	{
		RotateZ(globalPi / 4.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_I))
	{
		RotateZ(-(globalPi / 4.f * aDeltaTime));
	}

	if (myInputWrapper->KeyIsPressed(DIK_UP))
	{
		RotateX(-(globalPi / 4.f * aDeltaTime));
	}
	if (myInputWrapper->KeyIsPressed(DIK_DOWN))
	{
		RotateX(globalPi / 4.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_LEFT))
	{
		RotateY(-(globalPi / 4.f * aDeltaTime));
	}
	if (myInputWrapper->KeyIsPressed(DIK_RIGHT))
	{
		RotateY(globalPi / 4.f * aDeltaTime);
	}
}