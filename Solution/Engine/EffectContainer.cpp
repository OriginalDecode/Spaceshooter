#include "stdafx.h"

#include "Engine.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "FileWatcher.h"

Prism::Effect* Prism::EffectContainer::GetEffect(const std::string& aFilePath)
{
	auto it = myEffects.find(aFilePath);

	if (it == myEffects.end())
	{
		LoadEffect(aFilePath);
	}

	return myEffects[aFilePath];
}

void Prism::EffectContainer::LoadEffect(const std::string& aFilePath)
{
	Effect* newEffect = new Effect();
	newEffect->Init(aFilePath);

	myEffects[aFilePath] = newEffect;

	WATCH_FILE(aFilePath, Prism::EffectContainer::ReloadEffect);
}

void Prism::EffectContainer::ReloadEffect(const std::string& aFilePath)
{
	if (myEffects.find(aFilePath) == myEffects.end())
	{
		return;
	}

	myEffects[aFilePath]->Init(aFilePath);
}

void Prism::EffectContainer::Update(const float aDeltaTime)
{
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
	{
		it->second->UpdateTime(aDeltaTime);
	}
}