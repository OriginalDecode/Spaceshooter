#include "stdafx.h"

#include "Component.h"
#include "Entity.h"

void Entity::Update(float aDeltaTime)
{
	for (auto it = myComponents.begin(); it != myComponents.end(); ++it)
	{
		it->second->Update(aDeltaTime);
	}
}