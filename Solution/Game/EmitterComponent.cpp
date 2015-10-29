#include "stdafx.h"

#include "Camera.h"
#include "Constants.h"

#include "EmitterComponent.h"

#include <EmitterData.h>
#include <EmitterInstance.h>
#include "EmitterNote.h"
#include "Entity.h"
#include "Scene.h"

int EmitterComponent::myEmitterCount = 0;

EmitterComponent::EmitterComponent(Entity& aEntity)
	: Component(aEntity)
	, myEmitter(nullptr)
{
	myEmitterCount++;
}

EmitterComponent::~EmitterComponent()
{
	delete myEmitter;
	myEmitter = nullptr;
}

void EmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitter == nullptr, "Emitter were inited twice. Contact Linus Skold");
	myEmitter = new Prism::EmitterInstance();
	Prism::EmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitter->Initiate(data);
}

void EmitterComponent::Update(float aDeltaTime)
{
	myEmitter->Update(aDeltaTime, myEntity.myOrientation );
}

void EmitterComponent::Render()
{
	myEmitter->Render(&myEntity.GetScene().GetCamera());
}

eComponentType EmitterComponent::GetType()
{
	return eComponentType::EMITTER;
}

int EmitterComponent::GetEmitterCount()
{
	return myEmitterCount;
}

Prism::EmitterInstance* EmitterComponent::GetEmitter()
{
	return myEmitter;
}

void EmitterComponent::ReceiveNote(const EmitterNote& aNote)
{
	if (aNote.myType == EmitterNote::eType::BULLET)
	{
		myEmitter->ToggleActive();
	}
}
