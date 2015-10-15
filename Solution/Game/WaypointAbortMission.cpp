#include "stdafx.h"
#include "Entity.h"
#include "GUINote.h"
#include "Level.h"
#include "MissionNote.h"
#include "WaypointComponent.h"
#include "WaypointAbortMission.h"
#include "XMLReader.h"


WaypointAbortMission::WaypointAbortMission(Level& aLevel, Entity& aPlayer, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
	, myLevel(aLevel)
	, myPlayer(aPlayer)
{
	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "trigger");
	myTrigger = myLevel.AddTrigger(aReader, triggerElement);
	myTrigger->AddComponent<WaypointComponent>();


}


WaypointAbortMission::~WaypointAbortMission()
{
}

bool WaypointAbortMission::Update(float)
{

	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
	engine->PrintDebugText("Current mission: GO TO WAYPOINT!", { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });

	myPlayer.SendNote<GUINote>(GUINote(myTrigger->myOrientation.GetPos(), eGUINoteType::WAYPOINT));

	return !myTrigger->GetAlive();
}

void WaypointAbortMission::Start()
{
	myPlayer.SendNote(MissionNote(eMissionType::WAYPOINT, eMissionEvent::START));
}

void WaypointAbortMission::End()
{
	myPlayer.SendNote(MissionNote(eMissionType::WAYPOINT, eMissionEvent::END));
}
