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


ParticleEmitterComponent::ParticleEmitterComponent(Entity& aEntity)
	: Component(aEntity)
	, myEmitter(nullptr)
{
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	PostMaster::GetInstance()->SendMessage(DestroyEmitterMessage(this));

	delete myEmitter;
	myEmitter = nullptr;
}

void ParticleEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitter == nullptr, "Emitter were inited twice. Contact Linus Skold");
	myEmitter = new Prism::ParticleEmitterInstance();
	Prism::ParticleEmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitter->Initiate(data);
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

eComponentType ParticleEmitterComponent::GetType()
{
	return eComponentType::PARTICLE_EMITTER;
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
	}
}
