#include "stdafx.h"
#include "CollisionNote.h"
#include "Entity.h"
#include "PropComponent.h"


PropComponent::PropComponent(Entity& aEntity)
	: Component(aEntity)
{
}


PropComponent::~PropComponent()
{
}

void PropComponent::ReceiveNote(const CollisionNote& aNote)
{
	aNote.myEntity.Kill();
}
