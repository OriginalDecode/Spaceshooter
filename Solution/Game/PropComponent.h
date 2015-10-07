#pragma once
#include "Component.h"

class CollisionNote;
class Entity;

class PropComponent : public Component
{
public:
	PropComponent(Entity& aEntity);
	~PropComponent();

	void ReceiveNote(const CollisionNote& aNote) override;

private:

};

