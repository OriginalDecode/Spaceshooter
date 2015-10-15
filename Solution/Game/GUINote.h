#pragma once
#include <Vector.h>
#include "Enums.h"

struct GUINote
{
	GUINote(const CU::Vector3<float>& aPosition, eGUINoteType aType);

	const CU::Vector3<float> myPosition;
	const eGUINoteType myType;

private:
	void operator=(GUINote&) = delete;
};

inline GUINote::GUINote(const CU::Vector3<float>& aPosition, eGUINoteType aType)
	: myPosition(aPosition)
	, myType(aType)
{
}