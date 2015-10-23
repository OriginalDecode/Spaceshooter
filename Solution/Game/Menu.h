#pragma once

namespace Prism
{
	class Sprite;
}

namespace CommonUtilities
{
	class InputWrapper;
}

class Button;
class GameStateMessage;

class Menu
{
public:
	Menu(const std::string& aXMLPath);
	~Menu();
	
	void Render(CU::InputWrapper* anInputWrapper);

	void Update(CU::InputWrapper* anInputWrapper);

	void OnResize(int aWidth, int aHeight);

private:

	CU::GrowingArray<Button*> myButtons;
	Prism::Sprite* myBackground;
	Prism::Sprite* myCrosshair;
};

