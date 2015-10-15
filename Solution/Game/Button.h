#pragma once

namespace Prism
{
	class Model2D;
}

namespace tinyxml2
{
	class XMLElement;
}

class GameStateMessage;
class XMLReader;

class Button
{
public:
	Button();
	Button(XMLReader& aReader, tinyxml2::XMLElement* aButtonElement);
	~Button();

	void Render();
	void Update(const CU::Vector2<float>& aMousePos, const bool& aMouseIsPressed);

	void OnResize();

private:

	CU::Vector2<float> myOriginalPosition;
	CU::Vector2<float> myOriginalSize;

	CU::Vector2<float> myPosition;
	CU::Vector2<float> mySize;

	Prism::Model2D* myBackground;
	Prism::Model2D* myHoverBackground;
	GameStateMessage* myClickEvent;

	bool myIsHovered;
};

