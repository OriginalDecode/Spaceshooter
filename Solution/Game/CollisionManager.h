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
	void CheckAllCollisions(CollisionComponent* aComponent, int aFilter);
	void CheckCollision(CollisionComponent* aComponent, CU::GrowingArray<CollisionComponent*>& someOtherComponents);

	int myPlayerFilter;
	int myEnemyFilter;
	int myPlayerBulletFilter;
	int myEnemyBulletFilter;
	int myTriggerFilter;
	int myPropFilter;

	CU::GrowingArray<CollisionComponent*> myPlayers;
	CU::GrowingArray<CollisionComponent*> myEnemies;
	CU::GrowingArray<CollisionComponent*> myPlayerBullets;
	CU::GrowingArray<CollisionComponent*> myEnemyBullets;
	CU::GrowingArray<CollisionComponent*> myTriggers;
	CU::GrowingArray<CollisionComponent*> myProps;
};

