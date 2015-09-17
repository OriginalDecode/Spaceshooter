#include "stdafx.h"

#include "GraphicsComponent.h"
#include <Engine.h>
#include <Instance.h>
#include <Model.h>

GraphicsComponent::GraphicsComponent()
	: myInstance(nullptr)
{
	GraphicsComponent::myID = 0;
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{

}