#include "stdafx.h"

#include "ControllerComponent.h"
#include "Entity.h"
#include "TranslationMessage.h"

void ControllerComponent::MoveUp(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_UP, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::MoveDown(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_DOWN, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::MoveLeft(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_LEFT, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::MoveRight(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_RIGHT, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::MoveForward(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_FORWARD, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::MoveBackward(float aDistance)
{
	TranslationMessage msg(eTranslationType::MOVE_BACKWARD, aDistance);
	myEntity->SendMessage(msg);
}

void ControllerComponent::RotateX(float aAmount)
{
	TranslationMessage msg(eTranslationType::ROTATE_X, aAmount);
	myEntity->SendMessage(msg);
}

void ControllerComponent::RotateY(float aAmount)
{
	TranslationMessage msg(eTranslationType::ROTATE_Y, aAmount);
	myEntity->SendMessage(msg);
}

void ControllerComponent::RotateZ(float aAmount)
{
	TranslationMessage msg(eTranslationType::ROTATE_Z, aAmount);
	myEntity->SendMessage(msg);
}