#include "stdafx.h"
#include "Engine.h"
#include "KillAllMission.h"
#include "Level.h"


KillAllMission::KillAllMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
	, myLevel(aLevel)
{
}

bool KillAllMission::Update(float aDeltaTime, int aMissionIndex)
{
	aDeltaTime;
	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
	engine->PrintDebugText("Current mission: Kill all enemies!", { screenCenter.x - 300, (-(screenCenter.y) + screenCenter.y * 0.5f) - aMissionIndex * 25.f });

	return myLevel.GetEnemiesAlive() <= 0;
}
