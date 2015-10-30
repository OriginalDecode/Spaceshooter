#pragma once

#include "BaseModel.h"

struct ID3DX11EffectShaderResourceVariable;
struct D3D11_VIEWPORT;

namespace Prism
{
	class Effect;
	class Texture;

	class FullScreenHelper : public BaseModel
	{
	public:
		FullScreenHelper();
		~FullScreenHelper();

		void Process(Texture* aSource, Texture* aTarget, int aEffect);
		void CopyTexture(Texture* aSource, Texture* aTarget);
		void CombineTextures(Texture* aSourceA, Texture* aSourceB, Texture* aTarget);
		void RenderToScreen(Texture* aSource);

	private:
		struct CombineData
		{
			Effect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* mySourceA = nullptr;
			ID3DX11EffectShaderResourceVariable* mySourceB = nullptr;
		};
		struct RenderToTextureData
		{
			Effect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* mySource = nullptr;
		};
		struct BloomData
		{
			Effect* myBloomEffect;
			ID3DX11EffectShaderResourceVariable* myBloomVariable;
			ID3DX11EffectScalarVariable* myTexelWidthVariable;
			ID3DX11EffectScalarVariable* myTexelHeightVariable;
			Texture* myMiddleMan;
			Texture* myFinalTexture;

			Effect* myDownSampleEffect;
			ID3DX11EffectShaderResourceVariable* myDownSampleVariable;

			Texture* myDownSampleTextures[2];
		};

		
		
		void CreateCombineData();
		void CreateRenderToTextureData();
		void CreateBloomData();
		void CreateVertices();
		void ActivateBuffers();

		void Render(Effect* aEffect);
		void Render(Effect* aEffect, const std::string& aTechnique);

		void DownSample(Texture* aTarget, Texture* aSource, float aWidth, float aHeight);
		void BloomEffect(Texture* aSource);
		void DoBloom(Texture* aSource, Texture* aTarget);

		CombineData myCombineData;
		RenderToTextureData myRenderToTextureData;
		BloomData myBloomData;
		Texture* myProcessingTexture;
		D3D11_VIEWPORT myViewPort;
		float myClearColor[4];
	};
}

