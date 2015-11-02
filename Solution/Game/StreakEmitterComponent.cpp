#include "stdafx.h"

#include "Camera.h"
#include "Constants.h"

#include "StreakEmitterComponent.h"
#include <StreakEmitterData.h>
#include <StreakEmitterInstance.h>
#include "EmitterNote.h"
#include "DestroyEmitterMessage.h"
#include "Entity.h"
#include "PostMaster.h"
#include "Scene.h"


StreakEmitterComponent::StreakEmitterComponent(Entity& aEntity)
	: Component(aEntity)
	, myEmitter(nullptr)
{
}

StreakEmitterComponent::~StreakEmitterComponent()
{
	//PostMaster::GetInstance()->SendMessage(DestroyEmitterMessage(this));

	delete myEmitter;
	myEmitter = nullptr;
}

void StreakEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitter == nullptr, "Emitter were inited twice. Contact Linus Skold");
	Prism::StreakEmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitter = new Prism::StreakEmitterInstance(data);
}

void StreakEmitterComponent::Update(float aDeltaTime)
{
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Update(aDeltaTime);
	}
}

void StreakEmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		myEmitter->Render(myEntity.GetScene().GetCamera());
	}
}

eComponentType StreakEmitterComponent::GetType()
{
	return eComponentType::STREAK_EMITTER;
}

Prism::StreakEmitterInstance* StreakEmitterComponent::GetEmitter()
{
	if (this != nullptr)
		return myEmitter;

	return nullptr;

}

//void StreakEmitterComponent::ReceiveNote(const EmitterNote& aNote)
//{
//	if (aNote.myType == EmitterNote::eType::BULLET)
//	{
//		//myEmitter->ToggleActive(aNote.myIsActive);
//	}
//}
