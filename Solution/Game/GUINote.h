#pragma once
#include <Vector.h>
#include "Enums.h"

struct GUINote
{
	GUINote(const CU::Vector3<float>& aPosition, eGUINoteType aType);

	CU::Vector3<float> myPosition;
	eGUINoteType myType;
};

inline GUINote::GUINote(const CU::Vector3<float>& aPosition, eGUINoteType aType)
	: myPosition(aPosition)
	, myType(aType)
{
}