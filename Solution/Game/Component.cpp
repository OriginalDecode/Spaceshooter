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

void Component::ReceiveNote(const CollisionNote&)
{
}

void Component::ReceiveNote(const InputNote&)
{
}

void Component::ReceiveNote(const MissionNote& aMessage)
{
}

void Component::ReceiveNote(const PowerUpNote&)
{
}

void Component::ReceiveNote(const ShootNote&)
{
}

void Component::ReceiveNote(const SteeringTargetNote&)
{
}

void Component::ReceiveNote(const GUINote& aNote)
{
}
