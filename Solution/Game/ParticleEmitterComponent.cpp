#include "stdafx.h"

#include "Camera.h"
#include "Constants.h"

#include "ParticleEmitterComponent.h"
#include <ParticleEmitterData.h>
#include <ParticleEmitterInstance.h>
#include "EmitterNote.h"
#include "DestroyEmitterMessage.h"
#include "Entity.h"
#include "PostMaster.h"
#include "Scene.h"
#include <EmitterDataContainer.h>


ParticleEmitterComponent::ParticleEmitterComponent(Entity& aEntity)
	: Component(aEntity)
	, myEmitter(nullptr)
{
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
#ifndef DLL_EXPORT
	PostMaster::GetInstance()->SendMessage(DestroyEmitterMessage(this));
#endif

	delete myEmitter;
	myEmitter = nullptr;
}

void ParticleEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitter == nullptr, "Emitter were inited twice. Contact Linus Skold");
	myEmitter = new Prism::ParticleEmitterInstance();
	myEmitter->Initiate(Prism::Engine::GetInstance()->GetEmitterDataContainer()->GetParticleData(aPath));
}

void ParticleEmitterComponent::Update(float aDeltaTime)
{
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Update(aDeltaTime, myEntity.myOrientation);
	}
}

void ParticleEmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Render(&myEntity.GetScene().GetCamera());
	}
}

Prism::ParticleEmitterInstance* ParticleEmitterComponent::GetEmitter()
{
	if (this != nullptr)
		return myEmitter;

	return nullptr;

}

void ParticleEmitterComponent::ReceiveNote(const EmitterNote& aNote)
{
	if (aNote.myType == EmitterNote::eType::BULLET)
	{
		myEmitter->ToggleActive(aNote.myIsActive);
		myEmitter->ShouldLive(aNote.myShouldLive);
	}
}
