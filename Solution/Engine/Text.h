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
		void Render();

	private:
		void operator=(Text&) = delete;
		void ConstructBuffers();
		const Font& myFont;
		std::string myText;

		CU::GrowingArray<VertexPosUV> myVertices;
		CU::GrowingArray<int> myIndices;
	};
}

inline void Prism::Text::SetPosition(const CU::Vector2<float>& aPosition)
{
	myPosition = aPosition;
}