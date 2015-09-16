#include "stdafx.h"

#include <d3dx11effect.h>
#include "Effect.h"
#include "Surface.h"
#include "Texture.h"
#include "TextureContainer.h"

Surface::Surface()
{
	myTextures.Init(2);
	myShaderViews.Init(2);
}


Surface::~Surface()
{
}


bool Surface::SetTexture(const std::string& aResourceName, const std::string& aFileName, const bool aUseSRGB)
{
	aUseSRGB;

	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(aFileName);
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();

	if (shaderVar->IsValid() == false)
	{
		std::string errorMsg = "Failed to get ShaderResource: " + aResourceName;
		//DL_MESSAGE_BOX(errorMsg.c_str(), "Surface Error", MB_ICONWARNING);
		return false;
	}

	myTextures.Add(tex);
	myShaderViews.Add(shaderVar);

	return true;
}

bool Surface::SetTexture(const std::string& aResourceName, Texture* aTexture)
{
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();
	if (shaderVar->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ShaderResource", "Surface Error", MB_ICONWARNING);
		return false;
	}

	myTextures.Add(aTexture);
	myShaderViews.Add(shaderVar);

	return true;
}

void Surface::Activate()
{
	Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(myPrimitiveTopologyType);

	for (int i = 0; i < myShaderViews.Size(); ++i)
	{
		myShaderViews[i]->SetResource(myTextures[i]->GetShaderView());
	}
}