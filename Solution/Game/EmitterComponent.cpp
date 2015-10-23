#include "stdafx.h"

#include "Camera.h"

#include "EmitterComponent.h"

#include <EmitterData.h>
#include <EmitterInstance.h>

#include "Entity.h"
#include "Scene.h"

EmitterComponent::EmitterComponent(Entity& aEntity)
	: Component(aEntity)
{
	//Prism::EmitterData data;
	//data.LoadDataFile("Data/Resource/Particle/P_default_emitter.xml");

	//myEmitter = new Prism::EmitterInstance();
	//myEmitter->Initiate(data);
	//myEmitter->SetPosition({ 5, 5, 5 });
}

EmitterComponent::~EmitterComponent()
{
	delete myEmitter;
	myEmitter = nullptr;
}

void EmitterComponent::Init(std::string aPath)
{
	myXMLPath = aPath;

	myEmitter = new Prism::EmitterInstance();

	Prism::EmitterData data;
	data.LoadDataFile(myXMLPath.c_str());
	myEmitter->Initiate(data);
	//myEmitter->SetPosition(myEntity.myOrientation.GetPos());}
}

void EmitterComponent::Update(float aDeltaTime)
{
	//myEmitter->SetParent(myEntity.myOrientation);

	myEmitter->Update(aDeltaTime, myEntity.myOrientation);
}

void EmitterComponent::Render()
{
	myEmitter->Render(&myEntity.GetScene().GetCamera());
}

eComponentType EmitterComponent::GetType()
{
	return eComponentType::EMITTER;
}

