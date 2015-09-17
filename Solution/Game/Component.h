#pragma once
#include "ComponentEnums.h"

class Entity;
class Component
{
public:
	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveMessage(eMessage aMessage);


	void SetEntity(Entity* aEntity);

	static int GetID();

protected:
	static int myID;
	Entity* myEntity;
};

inline int Component::GetID()
{
	return myID;
}

inline void Component::SetEntity(Entity* aEntity)
{
	myEntity = aEntity;
}