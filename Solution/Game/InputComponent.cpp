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
		myEntity->SendMessage(eMessage::MOVE_UP);
	}
	if (myInputWrapper->KeyIsPressed(DIK_J))
	{
		myEntity->SendMessage(eMessage::MOVE_DOWN);
	}
	if (myInputWrapper->KeyIsPressed(DIK_H))
	{
		myEntity->SendMessage(eMessage::MOVE_LEFT);
	}
	if (myInputWrapper->KeyIsPressed(DIK_K))
	{
		myEntity->SendMessage(eMessage::MOVE_RIGHT);
	}
}