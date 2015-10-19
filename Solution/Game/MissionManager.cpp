#include "stdafx.h"

#include "CommonHelper.h"
#include <DL_Assert.h>
#include "Entity.h"
#include "KillAllAbortMission.h"
#include "KillAllMission.h"
#include "KillXEnemiesAbortMission.h"
#include "KillXEnemiesMission.h"
#include "Level.h"
#include "MissionManager.h"
#include "PostMaster.h"
#include <sstream>
#include "SurvivalMission.h"
#include "SurvivalAbortMission.h"
#include "WaypointMission.h"
#include "XMLReader.h"


MissionManager::MissionManager(Level& aLevel, Entity& aPlayer, const std::string& aFileToReadFrom)
	: myLevel(aLevel)
	, myPlayer(aPlayer)
	, myMissions(16)
	, myCurrentMission(0) 
	, myMissionsNotOrder(16)
	, myAllowedToStartNextMission(true)
	, myEndEventsActive(false)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::EVENT_QUEUE_EMPTY, this);
	XMLReader reader;
	reader.OpenDocument(aFileToReadFrom);

	tinyxml2::XMLElement* element = reader.ForceFindFirstChild("root");

	for (element = reader.ForceFindFirstChild(element, "mission"); element != nullptr;
		element = reader.FindNextElement(element, "mission"))
	{
		std::string type;
		reader.ForceReadAttribute(element, "type", type);
		type = CU::ToLower(type);
		int missionIndex = -1;
		reader.ForceReadAttribute(element, "index", missionIndex);
		if (type == "waypoint")
		{
			WaypointMission* waypoint = new WaypointMission(myLevel, myPlayer, reader, element);
			waypoint->SetIndex(missionIndex);
			myMissionsNotOrder.Add(waypoint);
		}
		else if (type == "killx")
		{
			KillXEnemiesMission* mission = new KillXEnemiesMission(myLevel, reader, element);
			mission->SetIndex(missionIndex);
			myMissionsNotOrder.Add(mission);
		}
		else if (type == "killxabort")
		{
			KillXEnemiesAbortMission* mission = new KillXEnemiesAbortMission(myLevel, reader, element);
			mission->SetIndex(missionIndex);
			myMissionsNotOrder.Add(mission);
		}
		else if (type == "killall")
		{
			KillAllMission* killAll = new KillAllMission(myLevel, reader, element);
			killAll->SetIndex(missionIndex);
			myMissionsNotOrder.Add(killAll);
		}
		else if (type == "killallabort")
		{
			KillAllAbortMission* killAllAbort = new KillAllAbortMission(myLevel, reader, element);
			killAllAbort->SetIndex(missionIndex);
			myMissionsNotOrder.Add(killAllAbort);
		}
		else if (type == "survival")
		{
			SurvivalMission* survival = new SurvivalMission(reader, element);
			survival->SetIndex(missionIndex);
			myMissionsNotOrder.Add(survival);
		}
		else if (type == "survivalabort")
		{
			SurvivalAbortMission* survivalAbort = new SurvivalAbortMission(reader, element);
			survivalAbort->SetIndex(missionIndex);
			myMissionsNotOrder.Add(survivalAbort);
		}
		else if (type == "defend")
		{
			DefendMission* mission = new DefendMission(reader, element);
			mission->SetIndex(missionIndex);
			myMissionsNotOrder.Add(mission);
		}
		else
		{
			std::string error = "Missiontype not recognized: " + type;
			DL_ASSERT(error.c_str());
		}
	}

	int currentIndex = 0;
	while (myMissions.Size() != myMissionsNotOrder.Size())
	{
		int prevIndex = currentIndex;
		for (int i = 0; i < myMissionsNotOrder.Size(); ++i)
		{
			if (myMissionsNotOrder[i]->GetIndex() == currentIndex)
			{
				++currentIndex;
				myMissions.Add(myMissionsNotOrder[i]);
				break;
			}
		}
		DL_ASSERT_EXP(prevIndex == currentIndex - 1, "Mission index " + std::to_string(currentIndex) + " not found.");
	}

	reader.CloseDocument();
}

MissionManager::~MissionManager()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::EVENT_QUEUE_EMPTY, this);
	myMissions.DeleteAll();
}

void MissionManager::Init()
{
	myMissions[myCurrentMission]->EventsStart();
	myMissions[myCurrentMission]->Start();
	myAllowedToStartNextMission = false;
	myEndEventsActive = false;
}

void MissionManager::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myCurrentMission < myMissions.Size(), "CurrentMission out of bounds!");
	std::stringstream ss;
	ss << myCurrentMission;
	Prism::Engine::GetInstance()->PrintDebugText(ss.str(), { 400, -370 });
	if (myEndEventsActive == false && myMissions[myCurrentMission]->Update(aDeltaTime) == true)
	{
		myAllowedToStartNextMission = myMissions[myCurrentMission]->EventsEnd();
		myEndEventsActive = true;
	}

	if (myEndEventsActive == true && myAllowedToStartNextMission == true)
	{
		myMissions[myCurrentMission]->End();
		++myCurrentMission;
		if (myCurrentMission == myMissions.Size())
		{
			myLevel.CompleteLevel();
		}
		else
		{
			myMissions[myCurrentMission]->EventsStart();
			myMissions[myCurrentMission]->Start();
			myAllowedToStartNextMission = false;
			myEndEventsActive = false;
		}
	}
}

void MissionManager::ReceiveMessage(const EventQueueEmptyMessage&)
{
	myAllowedToStartNextMission = true;
}