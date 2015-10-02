#include "stdafx.h"

#include "GraphicsComponent.h"
#include "Entity.h"
#include <Engine.h>
#include <EffectContainer.h>
#include <Instance.h>
#include <Model.h>
#include "ModelLoader.h"
#include <ModelProxy.h>
#include <Scene.h>


GraphicsComponent::GraphicsComponent(Entity& aEntity)
	: Component(aEntity)
	, myInstance(nullptr)
{
}

GraphicsComponent::~GraphicsComponent()
{
	myEntity.GetScene().RemoveInstance(myInstance);
	delete myInstance;
	myInstance = nullptr;
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{
	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(aModelPath
		, aEffectPath);

	myInstance = new Prism::Instance(*model);
	myInstance->SetOrientationPointer(myEntity.myOrientation);
}

void GraphicsComponent::InitGeometry(const Prism::MeshData& aMeshData)
{
	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadGeometry(aMeshData);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::InitCube(float aWidth, float aHeight, float aDepth)
{
	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadCube(aWidth, aHeight, aDepth);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	myInstance->SetOrientation(myEntity.myOrientation);
}

void GraphicsComponent::SetPosition(const CU::Vector3<float>& aPosition)
{
	myEntity.myOrientation.SetPos(aPosition);
	myInstance->SetOrientation(myEntity.myOrientation);
}

void GraphicsComponent::SetScale(const CU::Vector3<float>& aScale)
{
	myInstance->SetScale(aScale);
}