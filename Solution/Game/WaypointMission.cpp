#include "stdafx.h"
#include "Entity.h"
#include "Level.h"
#include "MissionNote.h"
#include "WaypointComponent.h"
#include "WaypointMission.h"
#include "WaypointNote.h"
#include "XMLReader.h"


WaypointMission::WaypointMission(Level& aLevel, Entity& aPlayer, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: myLevel(aLevel)
	, myPlayer(aPlayer)
{
	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "trigger");
	myTrigger = myLevel.AddTrigger(aReader, triggerElement);
	myTrigger->AddComponent<WaypointComponent>();


}


WaypointMission::~WaypointMission()
{
}

bool WaypointMission::Update(float aDeltaTime)
{

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
	engine->PrintDebugText("Current mission: GO TO WAYPOINT!", { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });

	myPlayer.SendNote<WaypointNote>(WaypointNote(myTrigger->myOrientation.GetPos()));

	return !myTrigger->GetAlive();
}

void WaypointMission::Start()
{
	myPlayer.SendNote(MissionNote(eMissionType::WAYPOINT, eMissionEvent::START));
}

void WaypointMission::End()
{
	myPlayer.SendNote(MissionNote(eMissionType::WAYPOINT, eMissionEvent::END));
}
