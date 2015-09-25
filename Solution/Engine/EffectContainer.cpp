#include "stdafx.h"

#include <d3dx11effect.h>
#include "Engine.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "FileWatcher.h"
#include "Texture.h"
#include "TextureContainer.h"

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

	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myCubeMap);
	ID3DX11EffectShaderResourceVariable* shaderVar = newEffect->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

	if (shaderVar->IsValid())
	{
		shaderVar->SetResource(tex->GetShaderView());
	}


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


	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myCubeMap);
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffects[aFilePath]->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

	if (shaderVar->IsValid())
	{
		shaderVar->SetResource(tex->GetShaderView());
	}
}

void Prism::EffectContainer::Update(const float aDeltaTime)
{
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
	{
		it->second->UpdateTime(aDeltaTime);
	}
}

void Prism::EffectContainer::SetCubeMap(const std::string& aFilePath)
{
	myCubeMap = aFilePath;
}