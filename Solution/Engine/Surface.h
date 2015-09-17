#pragma once
#include <D3DX11.h>
#include <GrowingArray.h>

class Texture;
class Effect;

struct ID3DX11EffectShaderResourceVariable;

class Surface
{
public:
	Surface();

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

	void Activate();

private:
	CU::GrowingArray<Texture*> myTextures;
	CU::GrowingArray<ID3DX11EffectShaderResourceVariable*> myShaderViews;

	D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopologyType;
	int myVertexStart;
	int myVertexCount;
	int myIndexCount;
	int myIndexStart;

	Effect* myEffect;
};

inline int Surface::GetVertexStart() const
{
	return myVertexStart;
}

inline void Surface::SetVertexStart(const int aStart)
{
	myVertexStart = aStart;
}

inline int Surface::GetVertexCount() const
{
	return myVertexCount;
}

inline void Surface::SetVertexCount(const int aCount)
{
	myVertexCount = aCount;
}

inline int Surface::GetIndexStart() const
{
	return myIndexStart;
}

inline void Surface::SetIndexStart(const int aStart)
{
	myIndexStart = aStart;
}

inline int Surface::GetIndexCount() const
{
	return myIndexCount;
}

inline void Surface::SetIndexCount(const int aCount)
{
	myIndexCount = aCount;
}

inline void Surface::SetEffect(Effect* aEffect)
{
	myEffect = aEffect;
}

inline void Surface::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology)
{
	myPrimitiveTopologyType = aTopology;
}
