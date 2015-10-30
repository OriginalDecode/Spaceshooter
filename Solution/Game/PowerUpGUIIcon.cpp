#include "stdafx.h"

#include "PowerUpGUIIcon.h"
#include "Sprite.h"

PowerUpGUIIcon::PowerUpGUIIcon(const std::string& aActivePath, const std::string& aInactivePath
		, const CU::Vector2<float>& aPosition, const bool& aActive, const float* aDuration)
	: myPosition(aPosition)
	, myActive(aActive)
	, myDuration(aDuration)
{
	CU::Vector2<float> size = { 64.f, 64.f };
	myActiveIcon = new Prism::Sprite(aActivePath, size, size * 0.5f);
	myInactiveIcon = new Prism::Sprite(aInactivePath, size, size * 0.5f);
	myTextPosition = myPosition + CU::Vector2<float>(size.x * 0.75f, 0.f);
}


PowerUpGUIIcon::~PowerUpGUIIcon()
{
	delete myInactiveIcon;
	delete myActiveIcon;
	myActiveIcon = nullptr;
	myInactiveIcon = nullptr;
}

void PowerUpGUIIcon::Render()
{
	if (myActive == true)
	{
		myActiveIcon->Render(myPosition);
		if (myDuration != nullptr)
		{
			//rendertext på min pos
			Prism::Engine::GetInstance()->PrintText(*myDuration, myTextPosition, Prism::eTextType::RELEASE_TEXT);
		}
	}
	else
	{
		myInactiveIcon->Render(myPosition);
	}
}
