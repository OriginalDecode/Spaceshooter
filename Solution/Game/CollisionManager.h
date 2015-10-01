#pragma once

#include "Enums.h"
#include <GrowingArray.h>

class CollisionComponent;

class CollisionManager
{
public:

	CollisionManager();
	~CollisionManager();

	void Add(CollisionComponent* aComponent, eEntityType aEnum);
	void Remove(CollisionComponent* aComponent, eEntityType aEnum);

	void Update();
	void CleanUp();

private:

	int myPlayerFilter;
	int myEnemyFilter;
	int myPlayerBulletFilter;

	CollisionComponent* myPlayer;
	CU::GrowingArray<CollisionComponent*> myEnemies;
	CU::GrowingArray<CollisionComponent*> myPlayerBullets;
};

