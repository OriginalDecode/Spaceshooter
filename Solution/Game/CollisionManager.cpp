#include "stdafx.h"

#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "HealthComponent.h"
#include <Intersection.h>

CollisionManager::CollisionManager()
	: myPlayer(nullptr)
	, myEnemies(16)
	, myPlayerBullets(16)
	, myPlayerFilter(0)
	, myEnemyFilter(0)
	, myPlayerBulletFilter(0)
{

	myPlayerFilter = eCollisionEnum::ENEMY | eCollisionEnum::ENEMY_BULLET | eCollisionEnum::TRIGGER;
	myPlayerBulletFilter = eCollisionEnum::ENEMY;
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::Add(CollisionComponent* aComponent, eCollisionEnum aEnum)
{
	switch (aEnum)
	{
	case eCollisionEnum::PLAYER:
		myPlayer = aComponent;
		break;
	case eCollisionEnum::ENEMY:
		myEnemies.Add(aComponent);
		break;
	case eCollisionEnum::PLAYER_BULLET:
		myPlayerBullets.Add(aComponent);
		break;
	case eCollisionEnum::ENEMY_BULLET:
		break;
	case eCollisionEnum::TRIGGER:
		break;
	default:
		break;
	}
}


void CollisionManager::Remove(CollisionComponent* aComponent, eCollisionEnum aEnum)
{
	switch (aEnum)
	{
	case eCollisionEnum::PLAYER:
		DL_ASSERT("Tried to Remove PLAYER from CollisionManager.");
		break;
	case eCollisionEnum::ENEMY:
		myEnemies.RemoveCyclic(aComponent);
		break;
	case eCollisionEnum::PLAYER_BULLET:
		myPlayerBullets.RemoveCyclic(aComponent);
		break;
	case eCollisionEnum::ENEMY_BULLET:
		break;
	case eCollisionEnum::TRIGGER:
		break;
	default:
		break;
	}
}

void CollisionManager::Update()
{

	for (int i = myPlayerBullets.Size() - 1; i >= 0; --i)
	{
		for (int e = myEnemies.Size() - 1; e >= 0; --e)
		{
			Entity& enemy = *myEnemies[e]->GetEntity();
			if (enemy.GetAlive() == true 
				&& CU::Intersection::SphereVsSphere(myPlayerBullets[i]->GetSphere(), myEnemies[e]->GetSphere()) == true)
			{
				Entity& bullet = *myPlayerBullets[i]->GetEntity();
				enemy.GetComponent<HealthComponent>()->RemoveHealth(bullet.GetComponent<BulletComponent>()->GetDamage());
				bullet.GetComponent<BulletComponent>()->SetActive(false);
				myPlayerBullets.RemoveCyclicAtIndex(i);
				break;
			}
		}
	}
}

void CollisionManager::CleanUp()
{
	for (int i = myEnemies.Size() - 1; i >= 0; --i)
	{
		if (myEnemies[i]->GetEntity()->GetAlive() == false)
		{
			myEnemies.RemoveCyclicAtIndex(i);
		}
	}
}
