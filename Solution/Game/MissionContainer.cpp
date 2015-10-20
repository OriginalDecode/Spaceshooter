#include "stdafx.h"

#include "DefendMission.h"
#include "KillAllAbortMission.h"
#include "KillAllMission.h"
#include "KillXEnemiesAbortMission.h"
#include "KillXEnemiesMission.h"
#include <sstream>
#include "MissionContainer.h"
#include "PostMaster.h"
#include "SurvivalMission.h"
#include "SurvivalAbortMission.h"
#include "WaypointMission.h"
#include "XMLReader.h"


MissionContainer::MissionContainer(Level& aLevel, Entity& aPlayer, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
	, myRequiredMissions(8)
	, myOptionalMissions(8)
	, myActiveMissions(16)
	, myEndingMissions(8)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::EVENT_QUEUE_EMPTY, this);

	CU::GrowingArray<Mission*> unorderedMissions(8);
	for (tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "mission"); element != nullptr;
		element = aReader.FindNextElement(element, "mission"))
	{
		std::string type;
		aReader.ForceReadAttribute(element, "type", type);
		type = CU::ToLower(type);
		int missionIndex = -1;
		aReader.ForceReadAttribute(element, "index", missionIndex);
		if (type == "waypoint")
		{
			WaypointMission* waypoint = new WaypointMission(aLevel, aPlayer, aReader, element);
			waypoint->SetIndex(missionIndex);
			unorderedMissions.Add(waypoint);
		}
		else if (type == "killx")
		{
			KillXEnemiesMission* mission = new KillXEnemiesMission(aLevel, aReader, element);
			mission->SetIndex(missionIndex);
			unorderedMissions.Add(mission);
		}
		else if (type == "killxabort")
		{
			KillXEnemiesAbortMission* mission = new KillXEnemiesAbortMission(aLevel, aReader, element);
			mission->SetIndex(missionIndex);
			unorderedMissions.Add(mission);
		}
		else if (type == "killall")
		{
			KillAllMission* killAll = new KillAllMission(aLevel, aReader, element);
			killAll->SetIndex(missionIndex);
			unorderedMissions.Add(killAll);
		}
		else if (type == "killallabort")
		{
			KillAllAbortMission* killAllAbort = new KillAllAbortMission(aLevel, aReader, element);
			killAllAbort->SetIndex(missionIndex);
			unorderedMissions.Add(killAllAbort);
		}
		else if (type == "survival")
		{
			SurvivalMission* survival = new SurvivalMission(aReader, element);
			survival->SetIndex(missionIndex);
			unorderedMissions.Add(survival);
		}
		else if (type == "survivalabort")
		{
			SurvivalAbortMission* survivalAbort = new SurvivalAbortMission(aReader, element);
			survivalAbort->SetIndex(missionIndex);
			unorderedMissions.Add(survivalAbort);
		}
		else if (type == "defend")
		{
			DefendMission* mission = new DefendMission(aReader, element, false);
			mission->SetIndex(missionIndex);
			unorderedMissions.Add(mission);
		}
		else if (type == "defendabort")
		{
			DefendMission* mission = new DefendMission(aReader, element, true);
			mission->SetIndex(missionIndex);
			unorderedMissions.Add(mission);
		}
		else
		{
			std::string error = "Missiontype not recognized: " + type;
			DL_ASSERT(error.c_str());
		}
	}

	int currentIndex = 0;
	while (myRequiredMissions.Size() != unorderedMissions.Size())
	{
		int prevIndex = currentIndex;
		for (int i = 0; i < unorderedMissions.Size(); ++i)
		{
			if (unorderedMissions[i]->GetIndex() == currentIndex)
			{
				++currentIndex;
				myRequiredMissions.Add(unorderedMissions[i]);
				myActiveMissions.Add(unorderedMissions[i]);
				break;
			}
		}
		DL_ASSERT_EXP(prevIndex == currentIndex - 1, "Mission index " + std::to_string(currentIndex) + " not found.");
	}
}

MissionContainer::~MissionContainer()
{
	myRequiredMissions.DeleteAll();
	myOptionalMissions.DeleteAll();
	PostMaster::GetInstance()->UnSubscribe(eMessageType::EVENT_QUEUE_EMPTY, this);
}

bool MissionContainer::Update(float aDeltaTime)
{
	for (int i = myActiveMissions.Size() - 1; i >= 0; --i)
	{
		if (myActiveMissions[i]->Update(aDeltaTime) == true)
		{
			if (myActiveMissions[i]->EventsEnd() == true)
			{
				myEndingMissions.Add(myActiveMissions[i]);
			}
			else
			{
				myActiveMissions[i]->End();
			}

			myActiveMissions.RemoveCyclicAtIndex(i);
		}
	}

	return myActiveMissions.Size() <= 0 && myEndingMissions.Size() <= 0;
}

void MissionContainer::Start()
{
	for (int i = 0; i < myActiveMissions.Size(); ++i)
	{
		myActiveMissions[i]->EventsStart();
		myActiveMissions[i]->Start();
	}
}

void MissionContainer::End()
{

}

void MissionContainer::ReceiveMessage(const EventQueueEmptyMessage&)
{
	for (int i = 0; i < myEndingMissions.Size(); ++i)
	{
		myEndingMissions[i]->End();
	}

	myEndingMissions.RemoveAll();
}
