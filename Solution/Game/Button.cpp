#include "stdafx.h"
#include "Button.h"
#include <Camera.h>
#include <Engine.h>
#include "GameStateMessage.h"
#include <Model2D.h>
#include "PostMaster.h"
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

	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "position"), "x", myOriginalPosition.x);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "position"), "y", myOriginalPosition.y);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "path", picPath);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "sizeX", myOriginalSize.x);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "picture"), "sizeY", myOriginalSize.y);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "hoveredPicture"), "path", picHoveredPath);
	aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "event", eventType);

	if (eventType == "level")
	{
		int levelID;
		aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "ID", levelID);
		myClickEvent = new GameStateMessage(eGameState::LOAD_GAME, levelID);
	}
	else if (eventType == "menu")
	{
		std::string menuID;
		aReader.ReadAttribute(aReader.FindFirstChild(aButtonElement, "onClick"), "ID", menuID);
		myClickEvent = new GameStateMessage(eGameState::LOAD_MENU, menuID);
	}
	
	myBackground = new Prism::Model2D;
	myBackground->Init(picPath, mySize);

	myHoverBackground = new Prism::Model2D;
	myHoverBackground->Init(picHoveredPath, mySize);

	myIsHovered = false;

	OnResize();
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

void Button::OnResize()
{
	float windowSizeX = Prism::Engine::GetInstance()->GetWindowSize().x;
	float windowSizeY = Prism::Engine::GetInstance()->GetWindowSize().y;
	float resolutionOffset = windowSizeY / windowSizeX;
	myPosition = myOriginalPosition * resolutionOffset;

	// ger lite weird resultat
	mySize = myOriginalSize * resolutionOffset;

	myBackground->SetSize(mySize);
	myHoverBackground->SetSize(mySize);
}