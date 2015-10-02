#pragma once

#include "Enums.h"
#include <GrowingArray.h>

class CollisionComponent;

class CollisionManager
{
public:

	CollisionManager();

	void Add(CollisionComponent* aComponent, eEntityType aEnum);
	void Remove(CollisionComponent* aComponent, eEntityType aEnum);

	void Update();
	void CleanUp();

private:

	int myPlayerFilter;
	int myEnemyFilter;
	int myPlayerBulletFilter;
	int myEnemyBulletFilter;

	CollisionComponent* myPlayer;
	CU::GrowingArray<CollisionComponent*> myEnemies;
	CU::GrowingArray<CollisionComponent*> myPlayerBullets;
	CU::GrowingArray<CollisionComponent*> myEnemyBullets;
};

