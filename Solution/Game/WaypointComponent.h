#pragma once
#include "Component.h"

class WaypointComponent : public Component
{
public:
	WaypointComponent(Entity& aEntity);

	void ReceiveNote(const CollisionNote& aNote) override;

	static int GetID();

private:
};

inline int WaypointComponent::GetID()
{
	return 13;
}