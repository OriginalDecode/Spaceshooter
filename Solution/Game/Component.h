#pragma once
#include "ComponentEnums.h"

class Entity;
class EnemiesTargetNote;
class ShootNote;
class InputNote;
class SteeringTargetNote;
class WaypointMessage;
class PowerUpNote;

class Component
{
public:
	Component(Entity& aEntity);

	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveNote(const ShootNote& aMessage);
	virtual void ReceiveNote(const InputNote& aMessage);
	virtual void ReceiveNote(const SteeringTargetNote& aMessage);
	virtual void ReceiveNote(const WaypointMessage& aMessage);
	virtual void ReceiveNote(const EnemiesTargetNote& aMessage);
	virtual void ReceiveNote(const PowerUpNote& aMessage);


	Entity& GetEntity();

	static int GetID();

protected:
	void operator=(Component&) = delete;

	Entity& myEntity;
};

inline int Component::GetID()
{
	return -1;
}

inline Entity& Component::GetEntity()
{
	return myEntity;
}