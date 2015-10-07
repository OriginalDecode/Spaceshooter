#include "stdafx.h"

#include "CommonHelper.h"
#include <DL_Assert.h>
#include "Entity.h"
#include "KillAllMission.h"
#include "Level.h"
#include "MissionManager.h"
#include <sstream>
#include "SurvivalMission.h"
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
			myMissions.Add(new WaypointMission(myLevel, myPlayer, reader, element));
		}
		else if (type == "killall")
		{
			myMissions.Add(new KillAllMission(myLevel));
		}
		else if (type == "survival")
		{
			myMissions.Add(new SurvivalMission(reader, element));
		}
		else
		{
			DL_ASSERT("Missiontype not recognized. %s", type.c_str());
		}

	}
	reader.CloseDocument();
	myMissions[myCurrentMission]->Start();
}

void MissionManager::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myCurrentMission < myMissions.Size(), "CurrentMission out of bounds!");
	std::stringstream ss;
	ss << myCurrentMission;
	Prism::Engine::GetInstance()->PrintDebugText(ss.str(), { 400, -370 });
	if (myMissions[myCurrentMission]->Update(aDeltaTime) == true)
	{
		myMissions[myCurrentMission]->End();
		++myCurrentMission;
		if (myCurrentMission == myMissions.Size())
		{
			myLevel.CompleteLevel();
		}
		else
		{
			myMissions[myCurrentMission]->Start();
		}
	}
}
