#pragma once
#include <D3DX11.h>
#include <GrowingArray.h>

struct ID3D11ShaderResourceView;
struct ID3DX11EffectShaderResourceVariable;

namespace Prism{
	class Texture;
	class Effect;

	class Surface
	{
	public:
		Surface();
		~Surface();

		int GetVertexStart() const;
		void SetVertexStart(int aStart);

		int GetVertexCount() const;
		void SetVertexCount(int aCount);

		int GetIndexStart() const;
		void SetIndexStart(int aStart);

		int GetIndexCount() const;
		void SetIndexCount(int aCount);

		void SetEffect(Effect* aEffect);
		void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology);

		bool SetTexture(const std::string& aResourceName, const std::string& aFileName, bool aUseSRGB);
		bool SetTexture(const std::string& aResourceName, Texture* aTexture);
		bool SetTexture(const std::string& aResourceName, ID3D11ShaderResourceView* aResource);

		void Activate();
		void ReloadSurface();

		bool GetEmissive() const;
		void SetEmissive();

		bool VerifyTextures(const std::string& aModelPath);

	private:
		CU::GrowingArray<Texture*> myTextures;
		CU::GrowingArray<ID3D11ShaderResourceView*> myShaderResources;
		CU::GrowingArray<ID3DX11EffectShaderResourceVariable*> myShaderResourceViews;
		CU::GrowingArray<std::string> myShaderResourceNames;
		CU::GrowingArray<std::string> myFilePaths;

		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopologyType;
		int myVertexStart;
		int myVertexCount;
		int myIndexCount;
		int myIndexStart;

		Effect* myEffect;
		bool myEmissive;
	};
}

inline int Prism::Surface::GetVertexStart() const
{
	return myVertexStart;
}

inline void Prism::Surface::SetVertexStart(const int aStart)
{
	myVertexStart = aStart;
}

inline int Prism::Surface::GetVertexCount() const
{
	return myVertexCount;
}

inline void Prism::Surface::SetVertexCount(const int aCount)
{
	myVertexCount = aCount;
}

inline int Prism::Surface::GetIndexStart() const
{
	return myIndexStart;
}

inline void Prism::Surface::SetIndexStart(const int aStart)
{
	myIndexStart = aStart;
}

inline int Prism::Surface::GetIndexCount() const
{
	return myIndexCount;
}

inline void Prism::Surface::SetIndexCount(const int aCount)
{
	myIndexCount = aCount;
}

inline void Prism::Surface::SetEffect(Effect* aEffect)
{
	myEffect = aEffect;
}

inline void Prism::Surface::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology)
{
	myPrimitiveTopologyType = aTopology;
}

inline bool Prism::Surface::GetEmissive() const
{
	return myEmissive;
}