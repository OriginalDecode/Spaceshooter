#pragma once

namespace Prism
{
	class Model2D;
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
	
	void Render(Prism::Camera* aCamera, CU::InputWrapper* anInputWrapper);

	void Update(CU::InputWrapper* anInputWrapper);

	void OnResize(int aWidth, int aHeight);

private:

	CU::GrowingArray<Button*> myButtons;
	Prism::Model2D* myBackground;
	Prism::Model2D* myCrosshair;
};

