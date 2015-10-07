#include "stdafx.h"
#include <sstream>
#include "SurvivalMission.h"
#include "XMLReader.h"


SurvivalMission::SurvivalMission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	tinyxml2::XMLElement* timeElement = aReader.ForceFindFirstChild(aElement, "time");
	aReader.ForceReadAttribute(timeElement, "seconds", mySurvivalTime);

}

bool SurvivalMission::Update(float aDeltaTime)
{
	std::stringstream ss;
	ss.precision(2);
	ss << "Survive for: " << myTime << " seconds";
	Prism::Engine::GetInstance()->PrintDebugText(ss.str(), { 400, -400 });
	myTime -= aDeltaTime;
	return myTime < 0.f;
}

void SurvivalMission::Start()
{
	myTime = mySurvivalTime;
}

void SurvivalMission::End()
{
	myTime = 0.f;
}
