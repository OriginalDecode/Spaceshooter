#include "stdafx.h"
#include "Engine.h"
#include "KillAllAbortMission.h"
#include "Level.h"
#include "XMLReader.h"


KillAllAbortMission::KillAllAbortMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
	, myLevel(aLevel)
{
	tinyxml2::XMLElement* enemiesLeftElement = aReader.ForceFindFirstChild(aElement, "enemiesleft");
	aReader.ForceReadAttribute(enemiesLeftElement, "value", myAbortEnemiesLeft);
}

bool KillAllAbortMission::Update(float aDeltaTime, int aMissionIndex, eMissionCategory aMissionCategory)
{
	aDeltaTime;
	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
	std::string message = "Current mission: Kill all (abort) enemies!";
	if (aMissionCategory == eMissionCategory::NOT_REQUIRED)
	{
		message += " (Optional)";
	}
	engine->PrintDebugText(message, { screenCenter.x - 300, (-(screenCenter.y) + screenCenter.y * 0.5f) - aMissionIndex * 25.f });

	return myLevel.GetEnemiesAlive() <= myAbortEnemiesLeft;
}
