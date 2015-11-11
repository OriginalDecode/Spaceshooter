#include "stdafx.h"

#include "LevelButton.h"
#include "ScoreIO.h"
#include <Sprite.h>

LevelButton::LevelButton()
{
}

LevelButton::LevelButton(XMLReader& aReader, tinyxml2::XMLElement* aButtonElement, int aLevelID)
	: Button(aReader, aButtonElement)
{
	mySize = CU::Vector2<float>(64.f, 64.f);
	myOptionalBadgeGrey = new Prism::Sprite("Data/Resource/Texture/Menu/BadgeGrey.dds", mySize, mySize * 0.5f);
	myOptionalBadge = new Prism::Sprite("Data/Resource/Texture/Menu/Badge.dds", mySize, mySize * 0.5f);
	myStarGrey = new Prism::Sprite("Data/Resource/Texture/Menu/StarGrey.dds", mySize, mySize * 0.5f);
	myStar = new Prism::Sprite("Data/Resource/Texture/Menu/Star.dds", mySize, mySize * 0.5f);

	myScore = ScoreIO::Load(aLevelID);
}

LevelButton::~LevelButton()
{
	delete myOptionalBadge;
	delete myOptionalBadgeGrey;
	delete myStarGrey;
	delete myStar;
	myOptionalBadge = nullptr;
	myOptionalBadgeGrey = nullptr;
	myStarGrey = nullptr;
	myStar = nullptr;
}

void LevelButton::Render()
{
	Button::Render();

	for (int i = 0; i < 3; ++i)
	{
		if (i < myScore.myStars)
		{
			myStar->Render({ myPosition.x + (mySize.x * (i - 1)), myPosition.y - mySize.y });
		}
		else
		{
			myStarGrey->Render({ myPosition.x + (mySize.x * (i - 1)), myPosition.y - mySize.y });
		}
	}

	for (int i = 0; i < myScore.myTotalOptional; ++i)
	{
		if (myScore.myCompletedOptional <= i)
		{
			myOptionalBadgeGrey->Render({ myPosition.x + (mySize.x * (i - 1)), myPosition.y - mySize.y * 2.2f });
		}
		else
		{
			myOptionalBadge->Render({ myPosition.x + (mySize.x * (i - 1)), myPosition.y - mySize.y * 2.2f });
		}
	}
}