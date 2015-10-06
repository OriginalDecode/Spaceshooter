#pragma once
#include "ComponentEnums.h"

class Entity;
class EnemiesTargetMessage;
class ShootMessage;
class InputMessage;
class SteeringTargetMessage;
class WaypointMessage;
class PowerUpMessage;

class Component
{
public:
	Component(Entity& aEntity);

	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveMessage(const ShootMessage& aMessage);
	virtual void ReceiveMessage(const InputMessage& aMessage);
	virtual void ReceiveMessage(const SteeringTargetMessage& aMessage);
	virtual void ReceiveMessage(const WaypointMessage& aMessage);
	virtual void ReceiveMessage(const EnemiesTargetMessage& aMessage);
	virtual void ReceiveMessage(const PowerUpMessage& aMessage);


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