#include "stdafx.h"
#include "Entity.h"
#include "Level.h"
#include "WaypointMission.h"
#include "XMLReader.h"


WaypointMission::WaypointMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement) 
	: myLevel(aLevel)
{
	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "trigger");
	myTrigger = myLevel.AddTrigger(aReader, triggerElement);
	//myTrigger->AddComponent<WaypointComponent>()->Init();
}


WaypointMission::~WaypointMission()
{
}

bool WaypointMission::Update(float aDeltaTime)
{
	return !myTrigger->GetAlive();
}
