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
	, myEmitterIndex(0)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DESTORY_EMITTER, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH, this);

	int index = 0;

	ReadListOfLists("Data/Resource/Particle/LI_emitter_lists.xml");	

	for (int i = 0; i < PREALLOCATED_EMITTER_SIZE; i++)
	{
		Prism::ParticleEmitterInstance* newEmitter = new Prism::ParticleEmitterInstance();
		newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index]));
		newEmitter->ToggleActive();
		myFireExplosion.Insert(i, newEmitter);

		newEmitter = new Prism::ParticleEmitterInstance();
		newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index + 1]));
		newEmitter->ToggleActive();
		mySmokeExplosion.Insert(i, newEmitter);

		newEmitter = new Prism::ParticleEmitterInstance();
		newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(myXMLPaths[index + 2]));
		newEmitter->ToggleActive();
		mySparkExplosion.Insert(i, newEmitter);
	}
}

EmitterManager::~EmitterManager()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DESTORY_EMITTER, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH, this);
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

	for (int i = 0; i < PREALLOCATED_EMITTER_SIZE; ++i)
	{
		if (myFireExplosion[i]->GetIsActive() == true)
		{
			myFireExplosion[i]->Update(aDeltaTime, aWorldMatrix);
			mySmokeExplosion[i]->Update(aDeltaTime, aWorldMatrix);
			mySparkExplosion[i]->Update(aDeltaTime, aWorldMatrix);

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

	for (int i = 0; i < PREALLOCATED_EMITTER_SIZE; ++i)
	{
		if (myFireExplosion[i]->GetIsActive() == true)
		{
			myFireExplosion[i]->Render(aCamera);
			mySmokeExplosion[i]->Render(aCamera);
			mySparkExplosion[i]->Render(aCamera);
		}
	}
}

void EmitterManager::ReceiveMessage(const DestroyEmitterMessage& aMessage)
{
	myEmitters.RemoveNonCyclic(aMessage.myParticleEmitterComponent);
}


void EmitterManager::ReceiveMessage(const SpawnExplosionMessage& aMessage)
{
	if (myEmitterIndex >= PREALLOCATED_EMITTER_SIZE)
	{
		myEmitterIndex = 0;
	}

	myFireExplosion[myEmitterIndex]->SetPosition(aMessage.myPosition);
	myFireExplosion[myEmitterIndex]->ToggleActive(true);

	mySmokeExplosion[myEmitterIndex]->SetPosition(aMessage.myPosition);
	mySmokeExplosion[myEmitterIndex]->ToggleActive(true);

	mySparkExplosion[myEmitterIndex]->SetPosition(aMessage.myPosition);
	mySparkExplosion[myEmitterIndex]->ToggleActive(true);

	myEmitterIndex++;
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
		if (entityPath != "")
		{
			ReadList(entityPath);
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