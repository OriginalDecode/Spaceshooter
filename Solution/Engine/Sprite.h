#pragma once

#include "BaseModel.h"

namespace Prism
{
	class Sprite : public BaseModel
	{
	public:
		Sprite(const std::string& aFileName, const CU::Vector2<float>& aSpriteSize
			, const CU::Vector2<float>& aHotSpot);

		void Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale = { 1.f, 1.f }
			, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });

		void SetSize(const CU::Vector2<float> aTextureSize);
		const CU::Vector2<float>& GetSize() const;

	private:

		void CreateVertices();

		CU::Vector2<float> mySize;
		CU::Vector2<float> myHotspot;
	};
}

inline void Prism::Sprite::SetSize(const CU::Vector2<float> aTextureSize)
{
	//myTextureSize = aTextureSize;
}

inline const CU::Vector2<float>& Prism::Sprite::GetSize() const
{
	return mySize;
}