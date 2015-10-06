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

void Component::ReceiveNote(const ShootNote&)
{
}

void Component::ReceiveNote(const InputNote& aMessage)
{
}

void Component::ReceiveNote(const SteeringTargetNote&)
{
}

void Component::ReceiveNote(const WaypointMessage&)
{
}

void Component::ReceiveNote(const EnemiesTargetNote&)
{
}

void Component::ReceiveNote(const PowerUpNote&)
{
}
