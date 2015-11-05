#pragma once
#include <string>
#include "BaseModel.h"

namespace Prism
{
	class Font;

	class Text : public BaseModel
	{
	public:
		Text(const Font& aFont);
		void SetPosition(const CU::Vector2<float>& aPosition);
		void SetText(const std::string& aText);
		void SetColor(const CU::Vector4<float>& aColor);
		void Render();
		void SetScale(const CU::Vector2<float>& aScale);

	private:
		void operator=(Text&) = delete;
		void ConstructBuffers();
		const Font& myFont;
		std::string myText;

		CU::GrowingArray<VertexPosUV> myVertices;
		CU::GrowingArray<int> myIndices;

		CU::Vector4<float> myColor;
	};
}

inline void Prism::Text::SetPosition(const CU::Vector2<float>& aPosition)
{
	myPosition.x = static_cast<float>(static_cast<int>(aPosition.x + 0.5f));
	myPosition.y = static_cast<float>(static_cast<int>(aPosition.y + 0.5f));
}

inline void Prism::Text::SetScale(const CU::Vector2<float>& aScale)
{
	myScale = aScale;
}

inline void Prism::Text::SetColor(const CU::Vector4<float>& aColor)
{
	myColor = aColor;
}