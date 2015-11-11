#pragma once

#include "StateEnums.h"

namespace Prism
{
	class Sprite;
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

	virtual void Render();
	eStateStatus Update(const CU::Vector2<float>& aMousePos, const bool& aMouseIsPressed);

	void OnResize();

protected:
	CU::Vector2<float> myPosition;

private:

	CU::Vector2<float> myOriginalPosition;
	CU::Vector2<float> myOriginalSize;

	CU::Vector2<float> mySize;

	Prism::Sprite* myBackground;
	Prism::Sprite* myHoverBackground;
	GameStateMessage* myClickEvent;

	bool myIsHovered;
	bool myBack;
	bool myQuit;
	bool myCalcFromCenter;
	bool myPostSoundEvent;
	std::string myWwiseEvent;
};

