#include "stdafx.h"
#include "DefendMessage.h"
#include "DefendMission.h"
#include "Engine.h"
#include "Entity.h"
#include "Level.h"
#include "PostMaster.h"
#include <sstream>
#include <XMLReader.h>


DefendMission::DefendMission(XMLReader& aReader, tinyxml2::XMLElement* aElement, bool aAbortMission)
	: Mission(aReader, aElement)
	, myRealTimeStart(0)
	, myVisualTimeStart(0)
	, myAbortMission(aAbortMission)
	, myEntityToDefend(nullptr)
{
	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "defend");
	aReader.ForceReadAttribute(element, "defendName", myNameToDefend);
	myNameToDefend = CU::ToLower(myNameToDefend);

	if (myAbortMission == true)
	{
		element = aReader.ForceFindFirstChild(aElement, "secondsvisual");
		aReader.ForceReadAttribute(element, "value", myVisualTimeStart);
		element = aReader.ForceFindFirstChild(aElement, "secondsreal");
		aReader.ForceReadAttribute(element, "value", myRealTimeStart);
	}
	else
	{
		element = aReader.ForceFindFirstChild(aElement, "seconds");
		aReader.ForceReadAttribute(element, "value", myRealTimeStart);
		myVisualTimeStart = myRealTimeStart;
	}
}

DefendMission::~DefendMission()
{
	if (PostMaster::GetInstance()->IsSubscribed(eMessageType::DEFEND, this) == true)
	{
		PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	}
}

void DefendMission::Start()
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->SendMessage<DefendMessage>(DefendMessage(DefendMessage::eType::NAME, myNameToDefend));
	myRealTime = myRealTimeStart;
	myVisualTime = myVisualTimeStart;
}

bool DefendMission::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myEntityToDefend != nullptr, "Could not find entity to defend: " + myNameToDefend);

	std::stringstream ss;
	ss.precision(2);
	if (myAbortMission == false)
	{
		ss << "Current mission: DEFEND for: " << myVisualTime << " seconds";
	}
	else
	{
		ss << "Current mission: DEFEND (abort) for: (visual) " << myVisualTime << " (real) " << myRealTime << " seconds";
	}

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);

	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });
	myRealTime -= aDeltaTime;
	myVisualTime -= aDeltaTime;
	return myRealTime <= 0.f;
}

void DefendMission::End()
{
	myRealTime = 0.f;
	myVisualTime = 0.f;
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
}

void DefendMission::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myEntityToDefend = aMessage.myEntity;
	}
}