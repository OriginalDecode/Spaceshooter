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
{
	for (int i = 0; i < static_cast<int>(eComponentType::_COUNT); ++i)
	{
		myComponents[i] = nullptr;
	}
}

Entity::~Entity()
{
	for (int i = 0; i < static_cast<int>(eComponentType::_COUNT); ++i)
	{
		delete myComponents[i];
	}
}

void Entity::Update(float aDeltaTime)
{
	for (int i = 0; i < static_cast<int>(eComponentType::_COUNT); ++i)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Update(aDeltaTime);
		}
	}
}

void Entity::Kill()
{
	DL_ASSERT_EXP(myAlive == true, "Tried to kill an entity thats allready dead.");

	myAlive = false;
}