#include "stdafx.h"
#include "EmitterInstance.h"
#include "EmitterManager.h"
#include "EmitterComponent.h"

EmitterManager::EmitterManager()
{
	myEmitters.Init(32);
}

EmitterManager::~EmitterManager()
{
}

void EmitterManager::AddEmitter(EmitterComponent* anEmitter)
{
	myEmitters.Add(anEmitter);
}

void EmitterManager::UpdateEmitters(float aDeltaTime)
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		if (myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Update(aDeltaTime);
		}
	}
}

void EmitterManager::RenderEmitters()
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		if (myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Render();
		}
	}
}
