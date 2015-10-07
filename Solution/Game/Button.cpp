#include "stdafx.h"
#include "Button.h"
#include <Model2D.h>
#include <Camera.h>
#include "PostMaster.h"
#include "GameStateMessage.h"
#include <tinyxml2.h>
#include <XMLReader.h>

Button::Button()
{
}

Button::Button(XMLReader& aReader, tinyxml2::XMLElement* aButtonElement)
{
	std::string picPath;
	std::string picHoveredPath;
	std::string eventType;

	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "position"), "x", myPosition.x);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "position"), "y", myPosition.y);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "path", picPath);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "sizeX", mySize.x);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "sizeY", mySize.y);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "hoveredPicture"), "path", picHoveredPath);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "event", eventType);

	if (eventType == "level")
	{
		int ID;
		aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "ID", ID);
		if (ID == 1)
		{
			myClickEvent = new GameStateMessage(eGameState::INGAME_STATE, "Data/script/level1.xml");
		}
		else if (ID == 2)
		{
			myClickEvent = new GameStateMessage(eGameState::INGAME_STATE, "Data/script/level1.xml", false);
		}
	}
	else if (eventType == "menu")
	{
		int ID;
		aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "ID", ID);
		if (ID == 1)
		{
			myClickEvent = new GameStateMessage(eGameState::MAIN_MENU_STATE);
		}
		else if (ID == 2)
		{
			myClickEvent = new GameStateMessage(eGameState::LEVEL_SELECT_STATE);
		}
	}
	
	myBackground = new Prism::Model2D;
	myBackground->Init(picPath, mySize);

	myHoverBackground = new Prism::Model2D;
	myHoverBackground->Init(picHoveredPath, mySize);

	myIsHovered = false;
}

Button::~Button()
{
	delete myBackground;
	delete myClickEvent;
	myBackground = nullptr;
	myClickEvent = nullptr;
}

void Button::Render(Prism::Camera* aCamera)
{
	if (myIsHovered == false)
	{
		myBackground->Render(*aCamera, myPosition.x, myPosition.y);
	}
	else
	{
		myHoverBackground->Render(*aCamera, myPosition.x, myPosition.y);
	}
}

void Button::Update(const CU::Vector2<float>& aMousePos, const bool& aMouseIsPressed)
{
	myIsHovered = false;

	if ((aMousePos.x + mySize.x / 2) >= myPosition.x && 
		(aMousePos.x + mySize.x / 2) <= myPosition.x + mySize.x &&
		(-aMousePos.y - mySize.y / 2) <= myPosition.y &&
		(-aMousePos.y - mySize.y / 2) >= myPosition.y - mySize.y)
	{
		if (myClickEvent != nullptr && aMouseIsPressed == true)
		{
			PostMaster::GetInstance()->SendMessage(*myClickEvent);
		}
		myIsHovered = true;
	}
}