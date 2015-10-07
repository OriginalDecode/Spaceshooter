#include "stdafx.h"

#include "CommonHelper.h"
#include <DL_Assert.h>
#include "Entity.h"
#include "KillAllMission.h"
#include "Level.h"
#include "MissionManager.h"
#include "WaypointMission.h"
#include "XMLReader.h"


MissionManager::MissionManager(Level& aLevel, Entity& aPlayer, const std::string& aFileToReadFrom)
	: myLevel(aLevel)
	, myPlayer(aPlayer)
	, myMissions(16)
	, myCurrentMission(0)
{
	XMLReader reader;
	reader.OpenDocument(aFileToReadFrom);

	tinyxml2::XMLElement* element = reader.ForceFindFirstChild("root");

	for (element = reader.ForceFindFirstChild(element, "mission"); element != nullptr;
		element = reader.FindNextElement(element, "mission"))
	{
		std::string type;
		reader.ForceReadAttribute(element, "type", type);
		type = CU::ToLower(type);
		if (type == "waypoint")
		{
			WaypointMission* mission = new WaypointMission(myLevel, reader, element);
			myMissions.Add(mission);
		}
		else if (type == "killall")
		{
			KillAllMission* mission = new KillAllMission(myLevel);
			myMissions.Add(mission);
		}
		else
		{
			DL_ASSERT("Missiontype not recognized. %s", type.c_str());
		}

	}

}

void MissionManager::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myCurrentMission < myMissions.Size(), "CurrentMission out of bounds!");
	if (myMissions[myCurrentMission]->Update(aDeltaTime) == true)
	{
		++myCurrentMission;
		if (myCurrentMission == myMissions.Size())
		{
			myLevel.CompleteLevel();
		}
	}
}
