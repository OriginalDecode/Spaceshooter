#pragma once
#include "Button.h"
#include "LevelScore.h"
#include <Vector.h>

class LevelButton : public Button
{
public:
	LevelButton();
	LevelButton(XMLReader& aReader, tinyxml2::XMLElement* aButtonElement, int aLevelID);
	~LevelButton();

	void Render() override;

private:
	SaveScore myScore;
	Prism::Sprite* myOptionalBadgeGrey;
	Prism::Sprite* myOptionalBadge;
	Prism::Sprite* myStarGrey;
	Prism::Sprite* myStar;

	CU::Vector2<float> mySize;
};

