#pragma once
#include "ComponentEnums.h"

class Entity;
class ShootMessage;
class InputMessage;
class SteeringTargetMessage;

class Component
{
public:
	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveMessage(const ShootMessage& aMessage);
	virtual void ReceiveMessage(const InputMessage& aMessage);
	virtual void ReceiveMessage(const SteeringTargetMessage& aMessage);

	void SetEntity(Entity* aEntity);

	static int GetID();

protected:
	Entity* myEntity;
};

inline int Component::GetID()
{
	return -1;
}

inline void Component::SetEntity(Entity* aEntity)
{
	myEntity = aEntity;
}