#include "stdafx.h"
#include "EffectContainer.h"
#include "Effect.h"

EffectContainer::EffectContainer()
{
}


EffectContainer::~EffectContainer()
{
}


Effect* EffectContainer::GetEffect(const std::string& aFilePath)
{
	auto it = myEffects.find(aFilePath);

	if (it == myEffects.end())
	{
		LoadEffect(aFilePath);
	}

	return myEffects[aFilePath];
}

void EffectContainer::LoadEffect(const std::string& aFilePath)
{
	Effect* newEffect = new Effect();
	newEffect->Init(aFilePath);

	myEffects[aFilePath] = newEffect;
}

void EffectContainer::Update(const float aDeltaTime)
{
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
	{
		it->second->UpdateTime(aDeltaTime);
	}
}