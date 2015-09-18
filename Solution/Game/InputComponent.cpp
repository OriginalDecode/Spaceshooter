#include "stdafx.h"
#include "ComponentEnums.h"
#include "Entity.h"
#include "InputComponent.h"
#include <InputWrapper.h>


InputComponent::InputComponent()
{
	myID = 1;
}

void InputComponent::Init(CU::InputWrapper& aInputWrapper)
{
	myInputWrapper = &aInputWrapper;
}

void InputComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	if (myInputWrapper->KeyIsPressed(DIK_U))
	{
		MoveUp();
	}
	if (myInputWrapper->KeyIsPressed(DIK_J))
	{
		MoveDown();
	}
	if (myInputWrapper->KeyIsPressed(DIK_H))
	{
		MoveLeft();
	}
	if (myInputWrapper->KeyIsPressed(DIK_K))
	{
		MoveRight();
	}
	if (myInputWrapper->KeyIsPressed(DIK_O))
	{
		MoveForward();
	}
	if (myInputWrapper->KeyIsPressed(DIK_L))
	{
		MoveBackward();
	}
}