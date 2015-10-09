#include "stdafx.h"
#include "CollisionNote.h"
#include "Entity.h"
#include "HealthComponent.h"
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
	aNote.myEntity.GetComponent<HealthComponent>()->RemoveHealth(10);
	//aNote.myEntity.Kill();
}
