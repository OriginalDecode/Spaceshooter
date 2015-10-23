#pragma once
#include <Vector.h>
#include "Enums.h"

class Entity;

struct GUINote
{
	GUINote(Entity* anEntity, eGUINoteType aType);
	GUINote(CU::Vector3<float> aPosition, eGUINoteType aType);

	const CU::Vector3<float> myPosition;
	const eGUINoteType myType;
	Entity* myEntity;

private:
	void operator=(GUINote&) = delete;
};

inline GUINote::GUINote(Entity* anEntity, eGUINoteType aType)
	: myType(aType)
	, myEntity(anEntity)
{
}

inline GUINote::GUINote(CU::Vector3<float> aPosition, eGUINoteType aType)
	: myType(aType)
	, myEntity(nullptr)
	, myPosition(aPosition)
{
}