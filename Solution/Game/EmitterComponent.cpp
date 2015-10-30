#include "stdafx.h"

#include "Camera.h"
#include "Constants.h"

#include "EmitterComponent.h"
#include <EmitterData.h>
#include <EmitterInstance.h>
#include "EmitterNote.h"
#include "EmitterMessage.h"
#include "Entity.h"
#include "PostMaster.h"
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
	PostMaster::GetInstance()->SendMessage(DestroyEmitterMessage(this));

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
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Update(aDeltaTime, myEntity.myOrientation);
	}
}

void EmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Render(&myEntity.GetScene().GetCamera());
	}
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
	if (this != nullptr)
		return myEmitter;

	return nullptr;

}

void EmitterComponent::ReceiveNote(const EmitterNote& aNote)
{
	if (aNote.myType == EmitterNote::eType::BULLET)
	{
		myEmitter->ToggleActive();
	}
}
