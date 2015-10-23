#pragma once

#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <string.h>
#include <StaticArray.h>

struct ID3D11BlendState;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectShaderResourceVariable;

namespace Prism
{
	class EffectListener;
	class Texture;

	class Effect
	{
	public:
		Effect();
		~Effect();

		ID3DX11Effect* GetEffect();
		ID3DX11EffectTechnique* GetTechnique();
		const std::string& GetFileName() const;
		bool Init(const std::string& aEffectFile);
		void SetScaleVector(const CU::Vector3<float>& aScaleVector);
		void SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix);
		void SetViewMatrix(const CU::Matrix44<float>& aViewMatrix);
		void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);
		void SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask = 0xFFFFFFFF);
		void SetTexture(Texture* aTexture);
		void SetPosAndScale(const CU::Vector2<float>& aPos
			, const CU::Vector2<float>& aScale = { 1.f, 1.f });

		void UpdateDirectionalLights(
			const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData);
		void UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData);
		void UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData);
		void UpdateTime(const float aDeltaTime);


		void AddListener(EffectListener* aListener);
		bool ReloadShader(const std::string& aFile);

	private:
		ID3DX11Effect* myEffect;
		ID3DX11EffectTechnique* myTechnique;

		ID3DX11EffectMatrixVariable* myProjectionMatrix;
		ID3DX11EffectMatrixVariable* myViewMatrix;
		ID3DX11EffectMatrixVariable* myWorldMatrix;

		ID3DX11EffectScalarVariable* myEyePosition;
		ID3DX11EffectScalarVariable* myTotalTime;

		ID3DX11EffectVectorVariable* myScaleVector;

		ID3DX11EffectVariable* myDirectionalLight;
		ID3DX11EffectVariable* myPointLight;
		ID3DX11EffectVariable* mySpotLight;

		ID3DX11EffectShaderResourceVariable	*myTexture;

		ID3DX11EffectVectorVariable* mySpritePosAndScale;
		CU::Vector4<float> mySpritePosAndScaleVector;
		
		std::string myFileName;

		CU::GrowingArray<EffectListener*> myEffectListeners;
	};
}

inline ID3DX11Effect* Prism::Effect::GetEffect()
{
	return myEffect;
}

inline ID3DX11EffectTechnique* Prism::Effect::GetTechnique()
{
	return myTechnique;
}

inline const std::string& Prism::Effect::GetFileName() const
{
	return myFileName;
}


inline void Prism::Effect::AddListener(EffectListener* aListener)
{
	myEffectListeners.Add(aListener);
}
