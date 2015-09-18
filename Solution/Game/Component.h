#pragma once
#include "ComponentEnums.h"

class Entity;
class TranslationMessage;

class Component
{
public:
	virtual void Init();
	virtual void Update(float aDeltaTime);
	virtual void ReceiveMessage(const TranslationMessage& aMessage);


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