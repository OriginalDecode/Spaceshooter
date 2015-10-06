#include "stdafx.h"
#include "Menu.h"
#include "Button.h"
#include <Model2D.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "GameStateMessage.h"
#include <XMLReader.h>

Menu::Menu(const std::string& aXMLPath)
	: myButtons(8)
{
	XMLReader reader;
	reader.OpenDocument(aXMLPath);

	std::string background;
	std::string crosshair;
	CU::Vector2<float> crosshairSize;

	tinyxml2::XMLElement* menuElement = reader.FindFirstChild("menu");

	reader.ReadAttribute(reader.FindFirstChild(menuElement, "background"), "path", background);
	reader.ReadAttribute(reader.FindFirstChild(menuElement, "crosshair"), "path", crosshair);
	reader.ReadAttribute(reader.FindFirstChild(menuElement, "crosshair"), "sizeX", crosshairSize.x);
	reader.ReadAttribute(reader.FindFirstChild(menuElement, "crosshair"), "sizeY", crosshairSize.y);

	myCrosshair = new Prism::Model2D;
	myCrosshair->Init(crosshair, crosshairSize);
	myBackground = new Prism::Model2D;
	myBackground->Init(background, { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) });

	tinyxml2::XMLElement* buttonElement = reader.FindFirstChild(menuElement, "button");
	for (; buttonElement != nullptr; buttonElement = reader.FindNextElement(buttonElement))
	{
		Button* newButton = new Button(reader, buttonElement);
		myButtons.Add(newButton);
	}
}

Menu::~Menu()
{
	myButtons.DeleteAll();
	delete myCrosshair;
	delete myBackground;
	myCrosshair = nullptr;
	myBackground = nullptr;
}

void Menu::Render(Prism::Camera* aCamera, CU::InputWrapper* anInputWrapper)
{
	myBackground->Render(*aCamera, 0.f, 0.f);
	myCrosshair->Render(*aCamera, anInputWrapper->GetMousePosition().x, -anInputWrapper->GetMousePosition().y);

	for (int i = 0; i < myButtons.Size(); i++)
	{
		myButtons[i]->Render(aCamera);
	}
}

void Menu::Update(CU::InputWrapper* anInputWrapper)
{
	bool isMouseClicked = anInputWrapper->MouseDown(0);
	CU::Vector2<float> mousePos = anInputWrapper->GetMousePosition();

	for (int i = 0; i < myButtons.Size(); i++)
	{
		myButtons[i]->Update(mousePos, isMouseClicked);
	}
}