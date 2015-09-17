#include "stdafx.h"
#include "InputComponent.h"


InputComponent::InputComponent()
{
	myID = 1;
}

void InputComponent::Init()
{

}

void InputComponent::Update(float aDeltaTime)
{
	OutputDebugStringA("InputComponent::Update\n");
}