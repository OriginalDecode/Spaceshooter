#pragma once
#include "Enums.h"

struct MissionStartNote
{
	MissionStartNote(eMissionType aType);

	eMissionType myType;
};

inline MissionStartNote::MissionStartNote(eMissionType aType)
	: myType(aType)
{
}