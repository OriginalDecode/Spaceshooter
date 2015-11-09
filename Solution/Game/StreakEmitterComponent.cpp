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
	, myEmitterData(nullptr)
	, myEmitters(8)
{
}

StreakEmitterComponent::~StreakEmitterComponent()
{
	myEmitters.DeleteAll();
	delete myEmitterData;
}

void StreakEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitterData== nullptr, "Emitter were inited twice. Contact Linus Skold");
	myEmitterData = new Prism::StreakEmitterData();
	myEmitterData->LoadDataFile(myXMLPath.c_str());

	CU::Matrix44<float> posLeft;
	//posLeft.SetPos(CU::Vector3<float>(-4.098f, 2.672f, 11.2789f));
	posLeft.SetPos(CU::Vector3<float>());
	//CU::Matrix44<float> posRight;
	//posRight.SetPos(CU::Vector3<float>(4.098f, 2.672f, 11.2789f));

	AddStreak(posLeft);
	//AddStreak(posRight);
}

void StreakEmitterComponent::Update(float aDeltaTime)
{
	if (myEntity.GetAlive() == true)
	{
		for (int i = 0; i < myEmitters.Size(); ++i)
		{
			myEmitters[i]->myEmitter->SetOrientation(myEmitters[i]->myOrientation * myEntity.myOrientation);
			myEmitters[i]->myEmitter->Update(aDeltaTime);
		}
	}
}

void StreakEmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		for (int i = 0; i < myEmitters.Size(); ++i)
		{
			myEmitters[i]->myEmitter->Render(&myEntity.GetScene().GetCamera());
		}
	}
}

void StreakEmitterComponent::AddStreak(const CU::Matrix44f aOrientation)
{
	Emitter* emitter = new Emitter();
	emitter->myEmitter = new Prism::StreakEmitterInstance(*myEmitterData);
	emitter->myOrientation = aOrientation;
	myEmitters.Add(emitter);
}

void StreakEmitterComponent::Reset()
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		myEmitters[i]->myEmitter->Reset();
	}
}


StreakEmitterComponent::Emitter::~Emitter()
{
	delete myEmitter;
}