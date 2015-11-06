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
	, myEmitterLeft(nullptr)
	, myEmitterRight(nullptr)
{
	myOrientationLeft.SetPos(CU::Vector3<float>(-3.f, 0, 0));
	myOrientationRight.SetPos(CU::Vector3<float>(3.f, 0, 0));
}

StreakEmitterComponent::~StreakEmitterComponent()
{
	//PostMaster::GetInstance()->SendMessage(DestroyEmitterMessage(this));

	delete myEmitterLeft;
	myEmitterLeft = nullptr;
	delete myEmitterRight;
	myEmitterRight = nullptr;
}

void StreakEmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	DL_ASSERT_EXP(myEmitter == nullptr, "Emitter were inited twice. Contact Linus Skold");
	Prism::StreakEmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitterLeft = new Prism::StreakEmitterInstance(data);
	myEmitterRight = new Prism::StreakEmitterInstance(data);
}

void StreakEmitterComponent::Update(float aDeltaTime)
{
	myEmitterLeft->SetOrientation(myOrientationLeft * myEntity.myOrientation);
	myEmitterRight->SetOrientation(myOrientationRight * myEntity.myOrientation);
	if (myEntity.GetAlive() == true)
	{
		myEmitterLeft->Update(aDeltaTime);
		myEmitterRight->Update(aDeltaTime);
	}
}

void StreakEmitterComponent::Render()
{
	if (myEntity.GetAlive() == true)
	{
		myEmitterLeft->Render(&myEntity.GetScene().GetCamera());
		myEmitterRight->Render(&myEntity.GetScene().GetCamera());
	}
}

Prism::StreakEmitterInstance* StreakEmitterComponent::GetEmitter()
{
	if (this != nullptr)
		return myEmitterLeft;

	return nullptr;

}

//void StreakEmitterComponent::ReceiveNote(const EmitterNote& aNote)
//{
//	if (aNote.myType == EmitterNote::eType::BULLET)
//	{
//		//myEmitter->ToggleActive(aNote.myIsActive);
//	}
//}
