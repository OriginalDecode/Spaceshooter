#pragma once
#include "Component.h"

struct CollisionNote;
class Entity;

class PropComponent : public Component
{
public:
	PropComponent(Entity& aEntity);
	~PropComponent();

	void ReceiveNote(const CollisionNote& aNote) override;

	static eComponentType GetType();
private:

};

inline eComponentType PropComponent::GetType()
{
	return eComponentType::PROP;
}

