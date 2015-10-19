#include "stdafx.h"
#include "DefendMission.h"
#include "Engine.h"
#include "Level.h"
#include "PostMaster.h"
#include <sstream>
#include <XMLReader.h>


DefendMission::DefendMission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
{
	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "defend");
	aReader.ForceReadAttribute(element, "name", myNameToDefend);

	element = aReader.ForceFindFirstChild(aElement, "seconds");
	aReader.ForceReadAttribute(element, "value", myTotalTime);
}

void DefendMission::Start()
{
	//PostMaster::GetInstance()->SendMessage<DefendMessage>(eMessageType::DEFEND);
	myTime = myTotalTime;
}

bool DefendMission::Update(float aDeltaTime)
{
	std::stringstream ss;
	ss.precision(2);
	ss << "Current mission: DEFEND for: " << myTime << " seconds";

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);

	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });
	myTime -= aDeltaTime;
	return myTime <= 0.f;
}

void DefendMission::End()
{
	myTime = 0.f;
}
