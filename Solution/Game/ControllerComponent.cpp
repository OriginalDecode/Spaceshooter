#include "stdafx.h"

#include "ControllerComponent.h"
#include "Entity.h"

void ControllerComponent::MoveUp()
{
	myEntity->SendMessage(eMessage::MOVE_UP);
}

void ControllerComponent::MoveDown()
{
	myEntity->SendMessage(eMessage::MOVE_DOWN);
}

void ControllerComponent::MoveLeft()
{
	myEntity->SendMessage(eMessage::MOVE_LEFT);
}

void ControllerComponent::MoveRight()
{
	myEntity->SendMessage(eMessage::MOVE_RIGHT);
}

void ControllerComponent::MoveForward()
{
	myEntity->SendMessage(eMessage::MOVE_FORWARD);
}

void ControllerComponent::MoveBackward()
{
	myEntity->SendMessage(eMessage::MOVE_BACKWARD);
}