#include "stdafx.h"
#include <sstream>
#include "SurvivalAbortMission.h"
#include "XMLReader.h"


SurvivalAbortMission::SurvivalAbortMission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
{
	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "secondsvisual");
	aReader.ForceReadAttribute(element, "value", myVisualTimeStart);
	element = aReader.ForceFindFirstChild(aElement, "secondsreal");
	aReader.ForceReadAttribute(element, "value", myRealTimeStart);
}

bool SurvivalAbortMission::Update(float aDeltaTime, int aMissionIndex)
{
	std::stringstream ss;
	ss.precision(2);
	ss << "Current mission: Survive for: " << myVisualTime << " seconds (abort)";

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);

	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, (-(screenCenter.y) + screenCenter.y * 0.5f) - aMissionIndex * 25.f });
	myVisualTime -= aDeltaTime;
	myRealTime -= aDeltaTime;
	return myRealTime <= 0;
}

void SurvivalAbortMission::Start()
{
	myVisualTime = myVisualTimeStart;
	myRealTime = myRealTimeStart;
}

void SurvivalAbortMission::End()
{
	myVisualTime = 0.f;
	myRealTime = 0.f;
}
