#include "stdafx.h"
#include <sstream>
#include "SurvivalMission.h"
#include "XMLReader.h"


SurvivalMission::SurvivalMission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
{
	tinyxml2::XMLElement* element= aReader.ForceFindFirstChild(aElement, "seconds");
	aReader.ForceReadAttribute(element, "value", mySurvivalTime);
}

bool SurvivalMission::Update(float aDeltaTime, int aMissionIndex)
{
	std::stringstream ss;
	ss.precision(2);
	ss << "Current mission: Survive for: " << myTime << " seconds";

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);

	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, (-(screenCenter.y) + screenCenter.y * 0.5f) - aMissionIndex * 25.f });
	myTime -= aDeltaTime;
	return myTime <= 0.f;
}

void SurvivalMission::Start()
{
	myTime = mySurvivalTime;
}

void SurvivalMission::End()
{
	myTime = 0.f;
}
