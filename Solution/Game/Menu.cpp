#include "stdafx.h"
#include "Menu.h"
#include "Button.h"
#include <Sprite.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "GameStateMessage.h"
#include <XMLReader.h>

Menu::Menu(const std::string& aXMLPath)
	: myButtons(8)
	, myMainMenu(false)
{
	XMLReader reader;
	reader.OpenDocument(aXMLPath);

	std::string background;
	std::string crosshair;
	CU::Vector2<float> crosshairSize;

	tinyxml2::XMLElement* menuElement = reader.FindFirstChild("menu");

	reader.ReadAttribute(menuElement, "mainMenu", myMainMenu);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "background"), "path", background);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "path", crosshair);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "sizeX", crosshairSize.x);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "sizeY", crosshairSize.y);

	myCrosshair = new Prism::Sprite(crosshair, crosshairSize, crosshairSize/2.f);
	CU::Vector2<float> screenSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) };
	myBackground = new Prism::Sprite(background, screenSize, screenSize / 2.f);

	tinyxml2::XMLElement* buttonElement = reader.FindFirstChild(menuElement, "button");
	for (; buttonElement != nullptr; buttonElement = reader.FindNextElement(buttonElement))
	{
		Button* newButton = new Button(reader, buttonElement);
		myButtons.Add(newButton);
	}
	reader.CloseDocument();
}

Menu::~Menu()
{
	myButtons.DeleteAll();
	delete myCrosshair;
	delete myBackground;
	myCrosshair = nullptr;
	myBackground = nullptr;
}

void Menu::Render(CU::InputWrapper* anInputWrapper)
{
	myBackground->Render({ (myBackground->GetSize().x / 2), -(myBackground->GetSize().y / 2) });

	for (int i = 0; i < myButtons.Size(); i++)
	{
		myButtons[i]->Render();
	}

	myCrosshair->Render({ anInputWrapper->GetMousePosition().x, -anInputWrapper->GetMousePosition().y });
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

void Menu::OnResize(int aWidth, int aHeight)
{
	if (myBackground != nullptr)
	{
		myBackground->SetSize( { float(aWidth), float(aHeight) });
	}

	for (int i = 0; i < myButtons.Size(); i++)
	{
		myButtons[i]->OnResize();
	}
}