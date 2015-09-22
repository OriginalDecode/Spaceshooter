#include "stdafx.h"

#include "GraphicsComponent.h"
#include "Entity.h"
#include <Engine.h>
#include <EffectContainer.h>
#include <Instance.h>
#include <Model.h>
#include "RefreshOrientationMessage.h"

GraphicsComponent::GraphicsComponent()
	: myInstance(nullptr)
	, myRenderSelf(false)
	, myCamera(nullptr)
{
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{
	Prism::Model* model = Prism::Engine::GetInstance()->LoadModel(aModelPath
		, Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectPath));

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::InitGeometry(const Prism::MeshData& aMeshData)
{
	Prism::Model* model = new Prism::Model();
	model->InitGeometry(aMeshData);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::InitCube(float aWidth, float aHeight, float aDepth)
{
	Prism::Model* model = new Prism::Model();
	model->InitCube(aWidth, aHeight, aDepth);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	if (myRenderSelf == true)
	{
		myInstance->Render(*myCamera);
	}
}

void GraphicsComponent::ReceiveMessage(const RefreshOrientationMessage&)
{
	myInstance->SetOrientation(myEntity->myOrientation);
}

void GraphicsComponent::SetPosition(const CU::Vector3<float>& aPosition)
{
	myEntity->myOrientation.SetPos(aPosition);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void GraphicsComponent::SetSelfRender(Prism::Camera* aCamera)
{
	myCamera = aCamera;
	myRenderSelf = true;
}