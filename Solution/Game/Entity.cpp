#include "stdafx.h"

#include "Component.h"
#include "Entity.h"

Entity::Entity(eEntityType aType, Prism::Scene& aScene, const std::string& aName)
	: myAlive(true)
	, myType(aType)
	, myScene(aScene)
	, myName(aName)
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

void Entity::Kill()
{
	DL_ASSERT_EXP(myAlive == true, "Tried to kill an entity thats allready dead.");

	myAlive = false;
}