#include "stdafx.h"

#include "GraphicsComponent.h"
#include <Engine.h>
#include <Instance.h>
#include <Model.h>

GraphicsComponent::GraphicsComponent()
	: myInstance(nullptr)
{
	myID = 0;
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{
	Prism::Model* model = Prism::Engine::GetInstance()->LoadModel(aModelPath
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect(aEffectPath));

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

}

void GraphicsComponent::ReceiveMessage(eMessage aMessage)
{
	CU::Vector3<float> pos = myInstance->GetPosition();
	float speed = 0.01f;
	if (aMessage == eMessage::MOVE_UP)
	{
		pos.y += speed;
		myInstance->SetPosition(pos);
	}
	if (aMessage == eMessage::MOVE_DOWN)
	{
		pos.y -= speed;
		myInstance->SetPosition(pos);
	}
	if (aMessage == eMessage::MOVE_LEFT)
	{
		pos.x -= speed;
		myInstance->SetPosition(pos);
	}
	if (aMessage == eMessage::MOVE_RIGHT)
	{
		pos.x += speed;
		myInstance->SetPosition(pos);
	}
}

void GraphicsComponent::SetPosition(const CU::Vector3<float>& aPosition)
{
	myInstance->SetPosition(aPosition);
}