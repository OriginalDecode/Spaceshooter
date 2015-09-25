#include "stdafx.h"

#include "Component.h"
#include "Entity.h"

Entity::Entity()
	:myEntityIsActive(false)
{
}


Entity::~Entity()
{
	for (auto it = myComponents.begin(); it != myComponents.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}

	myComponents.clear();
}

void Entity::Update(float aDeltaTime)
{
	for (auto it = myComponents.begin(); it != myComponents.end(); ++it)
	{
		it->second->Update(aDeltaTime);
	}
}