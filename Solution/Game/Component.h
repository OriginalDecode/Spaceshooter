#pragma once
#include "ComponentEnums.h"

class Entity;

struct CollisionNote;
struct EnemiesTargetNote;
struct GUINote;
struct InputNote;
struct MissionNote;
struct PowerUpNote;
struct ShootNote;
struct SteeringTargetNote;
struct WaypointNote;


class Component
{
public:
	Component(Entity& aEntity);

	virtual void Init();
	virtual void Update(float aDeltaTime);

	virtual void ReceiveNote(const CollisionNote& aMessage);
	virtual void ReceiveNote(const EnemiesTargetNote& aMessage);
	virtual void ReceiveNote(const InputNote& aMessage);
	virtual void ReceiveNote(const MissionNote& aMessage);
	virtual void ReceiveNote(const PowerUpNote& aNote);
	virtual void ReceiveNote(const ShootNote& aMessage);
	virtual void ReceiveNote(const SteeringTargetNote& aMessage);
	virtual void ReceiveNote(const WaypointNote& aMessage);
	virtual void ReceiveNote(const GUINote& aNote);

	Entity& GetEntity();

	static int GetID();

protected:
	void operator=(Component&) = delete;

	Entity& myEntity;
};

inline int Component::GetID()
{
	return -1;
}

inline Entity& Component::GetEntity()
{
	return myEntity;
}