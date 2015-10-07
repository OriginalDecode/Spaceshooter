#pragma once
#include "Enums.h"

struct MissionNote
{
	MissionNote(eMissionType aType, eMissionEvent aEvent);

	eMissionType myType;
	eMissionEvent myEvent;
};

inline MissionNote::MissionNote(eMissionType aType, eMissionEvent aEvent)
	: myType(aType)
	, myEvent(aEvent)
{
}