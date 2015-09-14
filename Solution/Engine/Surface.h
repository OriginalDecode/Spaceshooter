#pragma once
#include <D3DX11.h>
#include <string>
#include <GrowingArray.h>

class Texture;
class Effect;
struct ID3DX11EffectShaderResourceVariable;

class Surface
{
public:
	Surface();
	~Surface();

	int GetVertexStart() const;
	int GetVertexCount() const;
	int GetIndexStart() const;
	int GetIndexCount() const;

	void SetEffect(Effect* aEffect);
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology);
	void SetVertexStart(const int aStart);
	void SetVertexCount(const int aCount);
	void SetIndexStart(const int aStart);
	void SetIndexCount(const int aCount);

	bool SetTexture(const std::string& aResourceName, const std::string& aFileName, const bool aUseSRGB);
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

inline int Surface::GetVertexCount() const
{
	return myVertexCount;
}

inline int Surface::GetIndexStart() const
{
	return myIndexStart;
}

inline int Surface::GetIndexCount() const
{
	return myIndexCount;
}


inline void Surface::SetEffect(Effect* aEffect)
{
	myEffect = aEffect;
}

inline void Surface::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology)
{
	myPrimitiveTopologyType = aTopology;
}

inline void Surface::SetVertexStart(const int aStart)
{
	myVertexStart = aStart;
}

inline void Surface::SetVertexCount(const int aCount)
{
	myVertexCount = aCount;
}

inline void Surface::SetIndexStart(const int aStart)
{
	myIndexStart = aStart;
}

inline void Surface::SetIndexCount(const int aCount)
{
	myIndexCount = aCount;
}