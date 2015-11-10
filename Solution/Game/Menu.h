#pragma once

#include "StateEnums.h"

namespace Prism
{
	class Sprite;
}

namespace CU
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

	eStateStatus Update(float aDeltaTime, CU::InputWrapper* anInputWrapper);

	void OnResize(int aWidth, int aHeight);

	bool GetMainMenu() const;

private:
	bool myMainMenu;
	CU::GrowingArray<Button*> myButtons;
	Prism::Sprite* myBackground;
	Prism::Sprite* myCrosshair;
	CU::Vector2<float> myScreenSize;
	CU::Vector2<float> myBackgroundSize;
	CU::Vector2<float> myMousePosition;
	bool myRenderCenter;
	float myMouseSensitivty;
};

inline bool Menu::GetMainMenu() const
{
	return myMainMenu;
}