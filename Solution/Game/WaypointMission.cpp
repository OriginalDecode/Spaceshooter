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
	Prism::Engine::GetInstance()->PrintDebugText("GO TO WAYPOINT!", { 400, -400 });

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
