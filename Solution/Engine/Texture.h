#pragma once

#include <string>

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Prism
{
	class Texture
	{
	public:
		~Texture();

		void Init(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat);

		bool LoadTexture(const std::string& aFilePath);
		const std::string& GetFileName() const;
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11ShaderResourceView* GetDepthStencilShaderView();
		ID3D11RenderTargetView* GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();
		void Release();

		void Resize(float aWidth, float aHeight);

	private:
		void CreateDepthStencilView(float aWidth, float aHeight);

		std::string myFileName;
		ID3D11ShaderResourceView* myTexture;
		ID3D11RenderTargetView* myRenderTargetView;
		int myTextureFormat;

		ID3D11DepthStencilView* myDepthStencilView;
		ID3D11ShaderResourceView* myDepthStencilShaderView;

		bool IsValid(UINT aValue);
	};
}