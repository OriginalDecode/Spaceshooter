#pragma once

class Entity;

struct CollisionNote
{
	CollisionNote(Entity& anEntity);

	Entity& myEntity;
};

inline CollisionNote::CollisionNote(Entity& anEntity)
	: myEntity(anEntity)
{
}