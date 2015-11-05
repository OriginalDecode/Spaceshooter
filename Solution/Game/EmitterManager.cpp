#include "stdafx.h"

#include "DestroyEmitterMessage.h"
#include "EmitterManager.h"
#include <EmitterDataContainer.h>
#include "Entity.h"
#include "ParticleEmitterComponent.h"
#include "ParticleEmitterInstance.h"
#include "PostMaster.h"
#include "SpawnExplosionMessage.h"
#include <XMLReader.h>


EmitterManager::EmitterManager()
	: myEmitters(32)
	, myXMLPaths(32)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DESTORY_EMITTER, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EFFECT_ON_HIT, this);


	int index = 0;

	ReadListOfLists("Data/Resource/Particle/LI_emitter_lists.xml");

	for (int i = 0; i < EXPLOSION_DATA_SIZE; ++i)
	{
		for (int j = 0; j < PREALLOCATED_EMITTER_SIZE; ++j)
		{
			Prism::ParticleEmitterInstance* newEmitter = new Prism::ParticleEmitterInstance();
			newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index]));
			newEmitter->ToggleActive();
			myExplosions[i]->myFireExplosion.Insert(j, newEmitter);

			newEmitter = new Prism::ParticleEmitterInstance();
			newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index + 1]));
			newEmitter->ToggleActive();
			myExplosions[i]->mySmokeExplosion.Insert(j, newEmitter);

			newEmitter = new Prism::ParticleEmitterInstance();
			newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index + 2]));
			newEmitter->ToggleActive();
			myExplosions[i]->mySparkExplosion.Insert(j, newEmitter);
		}
		index += 3;
	}
}

EmitterManager::~EmitterManager()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DESTORY_EMITTER, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EFFECT_ON_HIT, this);

	myExplosions.DeleteAll();




}

void EmitterManager::AddEmitter(ParticleEmitterComponent* anEmitter)
{
	myEmitters.Add(anEmitter);
}

void EmitterManager::UpdateEmitters(float aDeltaTime, CU::Matrix44f aWorldMatrix)
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{

		if (myEmitters[i]->GetEntity().GetAlive() == true && myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Update(aDeltaTime);
		}
		else if (myEmitters[i]->GetEntity().GetAlive() == false)
		{
			myEmitters.RemoveNonCyclicAtIndex(i);
		}
	}

	for (int i = 0; i < EXPLOSION_DATA_SIZE; ++i)
	{
		for (int j = 0; j < PREALLOCATED_EMITTER_SIZE; ++j)
		{
			if (myExplosions[i]->myFireExplosion[j]->GetIsActive() == true)
			{
				myExplosions[i]->myFireExplosion[j]->Update(aDeltaTime, aWorldMatrix);
				myExplosions[i]->mySmokeExplosion[j]->Update(aDeltaTime, aWorldMatrix);
				myExplosions[i]->mySparkExplosion[j]->Update(aDeltaTime, aWorldMatrix);
			}
		}
	}

}

void EmitterManager::RenderEmitters(Prism::Camera* aCamera)
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		if (myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Render();
		}
	}

	for (int i = 0; i < EXPLOSION_DATA_SIZE; ++i)
	{
		for (int j = 0; j < PREALLOCATED_EMITTER_SIZE; ++j)
		{
			if (myExplosions[i]->myFireExplosion[j]->GetIsActive() == true)
			{
				myExplosions[i]->myFireExplosion[j]->Render(aCamera);
				myExplosions[i]->mySmokeExplosion[j]->Render(aCamera);
				myExplosions[i]->mySparkExplosion[j]->Render(aCamera);
			}
		}
	}
}

void EmitterManager::ReceiveMessage(const DestroyEmitterMessage& aMessage)
{
	myEmitters.RemoveNonCyclic(aMessage.myParticleEmitterComponent);
}


void EmitterManager::ReceiveMessage(const SpawnExplosionMessage& aMessage)
{
	if (aMessage.GetMessageType() == eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH)
	{
		EnemyExplosion(aMessage);
	}
	
	if (aMessage.GetMessageType() == eMessageType::SPAWN_EFFECT_ON_HIT)
	{
		OnHitEffect(aMessage);
	}
	
	if (aMessage.GetMessageType() == eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH)
	{
		AstroidExplosion(aMessage);
	}

	if (aMessage.GetMessageType() == eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH)
	{
		PropExplosion(aMessage);
	}

	if (aMessage.GetMessageType() == eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH)
	{
		RocketExplosion(aMessage);
	}
}

void EmitterManager::ReadListOfLists(std::string aPath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aPath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");
	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string entityPath = "";
		rootDocument.ForceReadAttribute(e, "src", entityPath);
		int ID;
		rootDocument.ForceReadAttribute(e, "ID", ID);
		if (entityPath != "")
		{
			if (ID == static_cast<int>(eExplosionID::ENEMY_EXPLOSION))
			{
				ReadList(entityPath);
				ExplosionData* newData = new ExplosionData(entityPath);
				myExplosions.Insert(ID, newData);
			}
			if (ID == static_cast<int>(eExplosionID::PROP_EXPLOSION))
			{
				ReadList(entityPath);
				ExplosionData* newData = new ExplosionData(entityPath);
				myExplosions.Insert(ID, newData);
			}
			if (ID == static_cast<int>(eExplosionID::ASTROID_EXPLOSION))
			{
				ReadList(entityPath);
				ExplosionData* newData = new ExplosionData(entityPath);
				myExplosions.Insert(ID, newData);
			}
			if (ID == static_cast<int>(eExplosionID::ROCKET_EXPLOSION))
			{
				ReadList(entityPath);
				ExplosionData* newData = new ExplosionData(entityPath);
				myExplosions.Insert(ID, newData);
			}
			if (ID == static_cast<int>(eExplosionID::ONHIT_EFFECT))
			{
				ReadList(entityPath);
				ExplosionData* newData = new ExplosionData(entityPath);
				myExplosions.Insert(ID, newData);
			}
		}
	}
	rootDocument.CloseDocument();
}

void EmitterManager::ReadList(std::string aPath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aPath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");
	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string entityPath = "";
		rootDocument.ForceReadAttribute(e, "src", entityPath);
		if (entityPath != "")
		{
			myXMLPaths.Add(entityPath);
		}
	}
	rootDocument.CloseDocument();
}

void EmitterManager::EnemyExplosion(const SpawnExplosionMessage& aMessage)
{
	int index = static_cast<int>(eExplosionID::ENEMY_EXPLOSION);
	int emitterIndex = myExplosions[index]->myEmitterIndex;

	myExplosions[index]->myFireExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->myFireExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySmokeExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySmokeExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySparkExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySparkExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->myEmitterIndex++;
}

void EmitterManager::PropExplosion(const SpawnExplosionMessage& aMessage)
{
	int index = static_cast<int>(eExplosionID::PROP_EXPLOSION);
	int emitterIndex = myExplosions[index]->myEmitterIndex;

	myExplosions[index]->myFireExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->myFireExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySmokeExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySmokeExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySparkExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySparkExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->myEmitterIndex++;
}

void EmitterManager::AstroidExplosion(const SpawnExplosionMessage& aMessage)
{
	int index = static_cast<int>(eExplosionID::ASTROID_EXPLOSION);
	int emitterIndex = myExplosions[index]->myEmitterIndex;

	myExplosions[index]->myFireExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->myFireExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySmokeExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySmokeExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySparkExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySparkExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->myEmitterIndex++;
}

void EmitterManager::RocketExplosion(const SpawnExplosionMessage& aMessage)
{
	int index = static_cast<int>(eExplosionID::ROCKET_EXPLOSION);
	int emitterIndex = myExplosions[index]->myEmitterIndex;

	myExplosions[index]->myFireExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->myFireExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySmokeExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySmokeExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySparkExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySparkExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->myEmitterIndex++;
}

void EmitterManager::OnHitEffect(const SpawnExplosionMessage& aMessage)
{
	int index = static_cast<int>(eExplosionID::ONHIT_EFFECT);
	int emitterIndex = myExplosions[index]->myEmitterIndex;

	myExplosions[index]->myFireExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->myFireExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySmokeExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySmokeExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->mySparkExplosion[emitterIndex]->SetPosition(aMessage.myPosition);
	myExplosions[index]->mySparkExplosion[emitterIndex]->ToggleActive(true);

	myExplosions[index]->myEmitterIndex++;
}