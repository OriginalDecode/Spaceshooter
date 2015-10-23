#include "stdafx.h"

#include <AudioInterface.h>
#include "Component.h"
#include "EnemyKilledMessage.h"
#include "Entity.h"
#include "PostMaster.h"

Entity::Entity(eEntityType aType, Prism::Scene& aScene, Prism::eOctreeType anOctreeType, const std::string& aName)
	: myAlive(true)
	, myType(aType)
	, myScene(aScene)
	, myOctreeType(anOctreeType)
	, myName(aName)
	, myPowerUpType(ePowerUpType::NO_POWERUP)
	, myAudioSFXID(-1)
{
	for (int i = 0; i < static_cast<int>(eComponentType::_COUNT); ++i)
	{
		myComponents[i] = nullptr;
	}
	myAudioSFXID = Prism::Audio::AudioInterface::GetInstance()->GetUniqueID();

	if (myType == eEntityType::PLAYER)
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Thrusters", myAudioSFXID);
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_ThrusterBoost", myAudioSFXID);
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
	if (myType == eEntityType::PLAYER)
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Stop_Thrusters", myAudioSFXID);
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Stop_ThrusterBoost", myAudioSFXID);
	}

	Prism::Audio::AudioInterface::GetInstance()->UnRegisterObject(myAudioSFXID);
}

void Entity::Update(float aDeltaTime)
{
	if (myType == eEntityType::PLAYER)
	{
		Prism::Audio::AudioInterface::GetInstance()->SetListenerPosition(myOrientation.GetPos().x
			, myOrientation.GetPos().y, myOrientation.GetPos().z);
	}
	else
	{
		Prism::Audio::AudioInterface::GetInstance()->SetPosition(myOrientation.GetPos().x
			, myOrientation.GetPos().y, myOrientation.GetPos().z, myAudioSFXID);
	}
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