#pragma once

#include <GrowingArray.h>



class CollisionComponent;

class CollisionManager
{
public:
	enum eCollisionEnum
	{
		PLAYER = 1,
		ENEMY = 2,
		PLAYER_BULLET = 4,
		ENEMY_BULLET = 8,
		TRIGGER = 16,

	};

	CollisionManager();
	~CollisionManager();

	void Add(CollisionComponent* aComponent, eCollisionEnum aEnum);

	void Update();


private:

	int myPlayerFilter;
	int myEnemyFilter;
	int myPlayerBulletFilter;

	CollisionComponent* myPlayer;
	CU::GrowingArray<CollisionComponent*> myEnemies;
	CU::GrowingArray<CollisionComponent*> myPlayerBullets;
};

