#include "stdafx.h"

#include "DestroyEmitterMessage.h"
#include "EmitterManager.h"
#include <EmitterDataContainer.h>
#include "Entity.h"
#include "ParticleEmitterComponent.h"
#include "ParticleEmitterInstance.h"
#include "PostMaster.h"
#include "SpawnExplosionMessage.h"

EmitterManager::EmitterManager()
	: myEmitters(32)
	, myEmitterIndex(0)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DESTORY_EMITTER, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ENEMY_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ASTROID_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_PROP_DEATH, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_EXPLOSION_ON_ROCKET_DEATH, this);


	for (int i = 0; i < PREALLOCATED_EMITTER_SIZE; i++)
	{
		Prism::ParticleEmitterInstance* newEmitter = new Prism::ParticleEmitterInstance();
		newEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData("Data/Resource/Particle/P_powerup_health.xml"));
		newEmitter->ToggleActive();
		myFireExplosion.Insert(i, newEmitter);
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
		}
	}

}

void EmitterManager::RenderEmitters(Prism::Camera* aCamera)
{
	Prism::Engine::GetInstance()->DisableZBuffer();
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
		}
	}
	Prism::Engine::GetInstance()->EnableZBuffer();
}

void EmitterManager::ReceiveMessage(const DestroyEmitterMessage& aMessage)
{
	myEmitters.RemoveNonCyclic(aMessage.myParticleEmitterComponent);
}


void EmitterManager::ReceiveMessage(const SpawnExplosionMessage& aMessage)
{
	myFireExplosion[myEmitterIndex]->SetPosition(aMessage.myPosition);
	myFireExplosion[myEmitterIndex]->ToggleActive();
}
