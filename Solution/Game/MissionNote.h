#pragma once
#include "Enums.h"

struct MissionNote
{
	MissionNote(eMissionType aType, eMissionEvent aEvent);

	const eMissionType myType;
	const eMissionEvent myEvent;
};

inline MissionNote::MissionNote(eMissionType aType, eMissionEvent aEvent)
	: myType(aType)
	, myEvent(aEvent)
{
}