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
	Model* model = Engine::GetInstance()->LoadModel(aModelPath
		, Engine::GetInstance()->GetEffectContainer().GetEffect(aEffectPath));

	myInstance = new Instance(*model);
}

void GraphicsComponent::Update(float aDeltaTime)
{

}