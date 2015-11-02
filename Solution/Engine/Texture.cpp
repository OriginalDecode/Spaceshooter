#include "stdafx.h"

#include <D3DX11tex.h>
#include <DL_Debug.h>
#include "Texture.h"


Prism::Texture::~Texture()
{
	myTexture->Release();
}

void Prism::Texture::Init(float aWidth, float aHeight, unsigned int aBindFlag
	, unsigned int aFormat)
{
	myTexture = nullptr;
	myRenderTargetView = nullptr;

	if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0 || (aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
		tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.ArraySize = 1;
		tempBufferInfo.Format = static_cast<DXGI_FORMAT>(aFormat);
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;

		ID3D11Texture2D* tempBuffer;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &tempBuffer);

		if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0)
		{
			hr = Engine::GetInstance()->GetDevice()->CreateShaderResourceView(tempBuffer, NULL, &myTexture);
			if (FAILED(hr))
				assert(0);
		}

		if ((aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
		{
			hr = Engine::GetInstance()->GetDevice()->CreateRenderTargetView(tempBuffer, NULL, &myRenderTargetView);
			if (FAILED(hr))
				assert(0);
		}
		tempBuffer->Release();

	}

	if ((aBindFlag & D3D11_BIND_DEPTH_STENCIL) > 0)
	{
		CreateDepthStencilView(aWidth, aHeight);
	}
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
	DL_ASSERT_EXP(myTexture != nullptr, "[Texture]: Tried to GetShaderView, but texture wasnt created with D3D11_BIND_SHADER_RESOURCE");
	return myTexture;
}

ID3D11RenderTargetView* Prism::Texture::GetRenderTargetView()
{
	DL_ASSERT_EXP(myRenderTargetView != nullptr, "[Texture]: Tried to GetRenderTargetView, but texture wasnt created with D3D11_BIND_RENDER_TARGET");
	return myRenderTargetView;
}

ID3D11DepthStencilView* Prism::Texture::GetDepthStencilView()
{
	DL_ASSERT_EXP(myDepthStencilView != nullptr, "[Texture]: Tried to GetDepthStencilView, but texture wasnt created with D3D11_BIND_DEPTH_STENCIL");
	return myDepthStencilView;
}

void Prism::Texture::Release()
{
	myTexture->Release();
}

void Prism::Texture::CreateDepthStencilView(float aWidth, float aHeight)
{
	D3D11_TEXTURE2D_DESC tempBufferInfo;
	tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
	tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
	tempBufferInfo.MipLevels = 1;
	tempBufferInfo.ArraySize = 1;
	tempBufferInfo.Format = DXGI_FORMAT_D32_FLOAT;
	tempBufferInfo.SampleDesc.Count = 1;
	tempBufferInfo.SampleDesc.Quality = 0;
	tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
	tempBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tempBufferInfo.CPUAccessFlags = 0;
	tempBufferInfo.MiscFlags = 0;

	ID3D11Texture2D* tempBuffer;
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &tempBuffer);


	tempBufferInfo.Format = DXGI_FORMAT_D32_FLOAT;
	hr = Engine::GetInstance()->GetDevice()->CreateDepthStencilView(tempBuffer, NULL, &myDepthStencilView);
	if (FAILED(hr))
		assert(0);

	tempBuffer->Release();
}

bool Prism::Texture::IsValid(UINT aValue)
{

	return aValue == 2 || aValue == 4 || aValue == 8 || aValue == 16 || aValue == 32 || aValue == 64 || aValue == 128
		|| aValue == 256 || aValue == 512 || aValue == 1024 || aValue == 2048 || aValue == 4096 || aValue == 8192 
		|| aValue == 1024 * 6;
}
