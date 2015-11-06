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
	, myEmitterInstance(nullptr)
	, myEmitters(8)
{
}

StreakEmitterComponent::~StreakEmitterComponent()
{
	delete myEmitterInstance;
	myEmitterInstance = nullptr;
}

void StreakEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitterInstance == nullptr, "Emitter were inited twice. Contact Linus Skold");
	Prism::StreakEmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitterInstance = new Prism::StreakEmitterInstance(data);

	CU::Matrix44<float> posLeft;
	posLeft.SetPos(CU::Vector3<float>(-4.098f, 2.672f, 11.2789f));
	CU::Matrix44<float> posRight;
	posRight.SetPos(CU::Vector3<float>(4.098f, 2.672f, 11.2789f));

	AddStreak(posLeft);
	AddStreak(posRight);
}

void StreakEmitterComponent::Update(float aDeltaTime)
{
	//myEmitterLeft->SetOrientation(myOrientationLeft * myEntity.myOrientation);
	//myEmitterRight->SetOrientation(myOrientationRight * myEntity.myOrientation);
	if (myEntity.GetAlive() == true)
	{
		for (int i = 0; i < myEmitters.Size(); ++i)
		{
			myEmitters[i].myEmitter->SetOrientation(myEmitters[i].myOrientation * myEntity.myOrientation);
			myEmitters[i].myEmitter->Update(aDeltaTime);
		}
	}
}

void StreakEmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		for (int i = 0; i < myEmitters.Size(); ++i)
		{
			myEmitters[i].myEmitter->Render(&myEntity.GetScene().GetCamera());
		}
	}
}

Prism::StreakEmitterInstance* StreakEmitterComponent::GetEmitter()
{
	if (this != nullptr)
		return myEmitterInstance;

	return nullptr;

}

//void StreakEmitterComponent::ReceiveNote(const EmitterNote& aNote)
//{
//	if (aNote.myType == EmitterNote::eType::BULLET)
//	{
//		//myEmitter->ToggleActive(aNote.myIsActive);
//	}
//}
