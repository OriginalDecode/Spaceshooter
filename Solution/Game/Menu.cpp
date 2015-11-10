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
	, myRenderCenter(false)
{
	XMLReader reader;
	reader.OpenDocument(aXMLPath);

	std::string background;
	std::string crosshair;
	CU::Vector2<float> crosshairSize;

	tinyxml2::XMLElement* menuElement = reader.FindFirstChild("menu");

	reader.ReadAttribute(menuElement, "mainMenu", myMainMenu);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "background"), "path", background);
	reader.ReadAttribute(reader.ForceFindFirstChild(menuElement, "background"), "sizeX", myBackgroundSize.x);
	reader.ReadAttribute(reader.ForceFindFirstChild(menuElement, "background"), "sizeY", myBackgroundSize.y);
	reader.ReadAttribute(reader.ForceFindFirstChild(menuElement, "background"), "renderCenter", myRenderCenter);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "path", crosshair);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "sizeX", crosshairSize.x);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(menuElement, "crosshair"), "sizeY", crosshairSize.y);

	myCrosshair = new Prism::Sprite(crosshair, crosshairSize, crosshairSize / 2.f);
	myScreenSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) };
	if (myBackgroundSize.x != 0 && myBackgroundSize.y != 0)
	{
		myBackground = new Prism::Sprite(background, myBackgroundSize, myBackgroundSize / 2.f);
	}
	else
	{
		myBackground = new Prism::Sprite(background, myScreenSize, myScreenSize / 2.f);
	}

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

void Menu::Render(CU::InputWrapper* anInputWrapper, bool aRenderButtons)
{
	if (myRenderCenter == true)
	{
		myBackground->Render({ (myScreenSize.x / 2.f), -(myScreenSize.y / 2.f) });
	}
	else
	{
		myBackground->Render({ (myBackground->GetSize().x / 2), -(myBackground->GetSize().y / 2) });
	}

	if (aRenderButtons == true)
	{
		for (int i = 0; i < myButtons.Size(); i++)
		{
			myButtons[i]->Render();
		}
	}

	myCrosshair->Render({ anInputWrapper->GetMousePosition().x, -anInputWrapper->GetMousePosition().y });
}

eStateStatus Menu::Update(CU::InputWrapper* anInputWrapper, bool aUpdateButtons)
{
	eStateStatus returnValue = eStateStatus::eKeepState;

	if (aUpdateButtons == true)
	{
		bool isMouseClicked = anInputWrapper->MouseDown(0);
		CU::Vector2<float> mousePos = anInputWrapper->GetMousePosition();

		for (int i = 0; i < myButtons.Size(); i++)
		{
			eStateStatus currentButton = myButtons[i]->Update(mousePos, isMouseClicked);
			if (currentButton == eStateStatus::ePopMainState)
			{
				returnValue = eStateStatus::ePopMainState;
			}
			else if (currentButton == eStateStatus::ePopSubState)
			{
				returnValue = eStateStatus::ePopSubState;
			}
		}
	}
	return returnValue;
}

void Menu::OnResize(int aWidth, int aHeight)
{
	myScreenSize = { float(aWidth), float(aHeight) };

	if (myBackground != nullptr)
	{
		if (myBackgroundSize.x != 0 && myBackgroundSize.y != 0)
		{
			myBackground->SetSize(myBackgroundSize, myBackgroundSize / 2.f);
		}
		else
		{
			myBackground->SetSize(myScreenSize, myScreenSize / 2.f);
		}

	}

	for (int i = 0; i < myButtons.Size(); i++)
	{
		myButtons[i]->OnResize();
	}
}