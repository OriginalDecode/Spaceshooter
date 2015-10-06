#include "stdafx.h"
#include "Component.h"

Component::Component(Entity& aEntity)
	: myEntity(aEntity)
{

}

void Component::Init()
{
}

void Component::Update(float)
{
}

void Component::ReceiveMessage(const ShootMessage&)
{
}

void Component::ReceiveMessage(const InputMessage& aMessage)
{
}

void Component::ReceiveMessage(const SteeringTargetMessage&)
{
}

void Component::ReceiveMessage(const PowerUpMessage& aMessage)
{
}
