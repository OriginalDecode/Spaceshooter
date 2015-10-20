#include "stdafx.h"

#include <D3DX11tex.h>
#include <DL_Debug.h>
#include "Texture.h"


Prism::Texture::~Texture()
{
	myTexture->Release();
}

bool Prism::Texture::LoadTexture(const std::string& aFilePath)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice(), aFilePath.c_str()
		, NULL, NULL, &myTexture, NULL);
	
	if (FAILED(hr) == S_OK)
	{
		ID3D11Resource* resource = nullptr;
		myTexture->GetResource(&resource);
		ID3D11Texture2D* texture2D = reinterpret_cast<ID3D11Texture2D*>(resource);
		D3D11_TEXTURE2D_DESC* texture2DDEsc = new D3D11_TEXTURE2D_DESC;
		texture2D->GetDesc(texture2DDEsc);
		UINT width = texture2DDEsc->Width;
		UINT height = texture2DDEsc->Height;
		texture2D->Release();
		delete texture2DDEsc;

		std::string errorMessage = "Texturesize not power of 2: [" + aFilePath + "].";
		DL_ASSERT_EXP(IsValid(height) && IsValid(width), errorMessage.c_str());
	}
	myFileName = aFilePath;
	if (FAILED(hr) != S_OK)
	{
		RESOURCE_LOG("Failed to load: %s, trying to use MissingTexture-texture.", aFilePath.c_str());
		hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice()
			, "Data/Resource/Texture/T_missing_texture.dds", NULL, NULL, &myTexture, NULL);
		myFileName = "Data/Resource/Texture/T_missing_texture.dds";
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("[Texture]: Failed to load MissingTexture-texture: Data/Resource/Texture/T_missing_texture.dds");
		}
	}

	return true;
}

const std::string& Prism::Texture::GetFileName() const
{
	return myFileName;
}

ID3D11ShaderResourceView* Prism::Texture::GetShaderView()
{
	return myTexture;
}

bool Prism::Texture::IsValid(UINT aValue)
{

	return aValue == 2 || aValue == 4 || aValue == 8 || aValue == 16 || aValue == 32 || aValue == 64 || aValue == 128
		|| aValue == 256 || aValue == 512 || aValue == 1024 || aValue == 2048 || aValue == 4096 || aValue == 8192 
		|| aValue == 1024 * 6;
}
