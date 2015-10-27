#include "stdafx.h"

#include <AudioInterface.h>
#include "Component.h"
#include "EnemyKilledMessage.h"
#include "Entity.h"
#include "PostMaster.h"
#include "SoundNote.h"

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
	if (myType == eEntityType::ENEMY)
	{
		PostMaster::GetInstance()->SendMessage<EnemyKilledMessage>(EnemyKilledMessage());
	}
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
	if (myComponents[static_cast<int>(eComponentType::SOUND)] != nullptr)
	{
		SendNote<SoundNote>(SoundNote(eSoundNoteType::PLAY, "Play_ShipExplosion"));
	}
	myAlive = false;
}

void Entity::Reset()
{
	myOrientation = myOriginalOrientation;
	myAlive = true;
	for (int i = 0; i < static_cast<int>(eComponentType::_COUNT); ++i)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Reset();
		}
	}
}