#include "stdafx.h"

#include "Component.h"
#include "Entity.h"

Entity::Entity(eEntityType aType, Prism::Scene& aScene, Prism::eOctreeType anOctreeType, const std::string& aName)
	: myAlive(true)
	, myType(aType)
	, myScene(aScene)
	, myOctreeType(anOctreeType)
	, myName(aName)
	, myPowerUpType(ePowerUpType::NO_POWERUP)
	, myComponentsArray(4)
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

	myComponentsArray.RemoveAll();
}

void Entity::Update(float aDeltaTime)
{
	for (int i = 0; i < myComponentsArray.Size(); ++i)
	{
		myComponentsArray[i]->Update(aDeltaTime);
	}
}

void Entity::Kill()
{
	DL_ASSERT_EXP(myAlive == true, "Tried to kill an entity thats allready dead.");

	myAlive = false;
}