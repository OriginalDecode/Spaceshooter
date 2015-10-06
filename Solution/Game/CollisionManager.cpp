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
	, myEnemyBullets(16)
	, myTriggers(16)
	, myPlayerFilter(0)
	, myEnemyFilter(0)
	, myPlayerBulletFilter(0)
	, myEnemyBulletFilter(0)
	, myTriggerFilter(0)
{

	myPlayerFilter = eEntityType::ENEMY | eEntityType::ENEMY_BULLET | eEntityType::TRIGGER;
	myPlayerBulletFilter = eEntityType::ENEMY;
	myEnemyBulletFilter = eEntityType::PLAYER;
	myTriggerFilter = eEntityType::PLAYER;
}

void CollisionManager::Add(CollisionComponent* aComponent, eEntityType aEnum)
{
	switch (aEnum)
	{
	case eEntityType::PLAYER:
		myPlayer = aComponent;
		break;
	case eEntityType::ENEMY:
		myEnemies.Add(aComponent);
		break;
	case eEntityType::PLAYER_BULLET:
		myPlayerBullets.Add(aComponent);
		break;
	case eEntityType::ENEMY_BULLET:
		myEnemyBullets.Add(aComponent);
		break;
	case eEntityType::TRIGGER:
		myTriggers.Add(aComponent);
		break;
	default:
		DL_ASSERT("Tried to Add invalid EntityType to CollisionManager.");
		break;
	}
}


void CollisionManager::Remove(CollisionComponent* aComponent, eEntityType aEnum)
{
	switch (aEnum)
	{
	case eEntityType::PLAYER:
		DL_ASSERT("Tried to Remove PLAYER from CollisionManager.");
		break;
	case eEntityType::ENEMY:
		myEnemies.RemoveCyclic(aComponent);
		break;
	case eEntityType::PLAYER_BULLET:
		myPlayerBullets.RemoveCyclic(aComponent);
		break;
	case eEntityType::ENEMY_BULLET:
		myEnemyBullets.RemoveCyclic(aComponent);
		break;
	case eEntityType::TRIGGER:
		myTriggers.RemoveCyclic(aComponent);
		break;
	default:
		DL_ASSERT("Tried to Remove invalid EntityType to CollisionManager.");
		break;
	}
}

void CollisionManager::Update()
{
	for (int i = myPlayerBullets.Size() - 1; i >= 0; --i)
	{
		for (int e = myEnemies.Size() - 1; e >= 0; --e)
		{
			Entity& enemy = myEnemies[e]->GetEntity();
			if (enemy.GetAlive() == true 
				&& CU::Intersection::SphereVsSphere(myPlayerBullets[i]->GetSphere(), myEnemies[e]->GetSphere()) == true)
			{
				Entity& bullet = myPlayerBullets[i]->GetEntity();
				enemy.GetComponent<HealthComponent>()->RemoveHealth(bullet.GetComponent<BulletComponent>()->GetDamage());
				bullet.GetComponent<BulletComponent>()->SetActive(false);
				myPlayerBullets.RemoveCyclicAtIndex(i);
				break;
			}
		}
	}

	for (int i = myEnemyBullets.Size() - 1; i >= 0; --i)
	{
		Entity& bullet = myEnemyBullets[i]->GetEntity();
		Entity& player = myPlayer->GetEntity();

		if (player.GetAlive() == true
			&& CU::Intersection::SphereVsSphere(myPlayer->GetSphere(), myEnemyBullets[i]->GetSphere()) == true)
		{
			player.GetComponent<HealthComponent>()->RemoveHealth(bullet.GetComponent<BulletComponent>()->GetDamage());
			bullet.GetComponent<BulletComponent>()->SetActive(false);
			myEnemyBullets.RemoveCyclicAtIndex(i);
		}
	}
}

void CollisionManager::CleanUp()
{
	for (int i = myEnemies.Size() - 1; i >= 0; --i)
	{
		if (myEnemies[i]->GetEntity().GetAlive() == false)
		{
			myEnemies.RemoveCyclicAtIndex(i);
		}
	}
	for (int i = myTriggers.Size() - 1; i >= 0; --i)
	{
		if (myTriggers[i]->GetEntity().GetAlive() == false)
		{
			myTriggers.RemoveCyclicAtIndex(i);
		}
	}
}

void CollisionManager::CheckCollision(CollisionComponent* aComponent, int aFilter)
{
	//Entity& entity = aComponent->GetEntity();
	//
	//if (aFilter & eEntityType::PLAYER)
	//{
	//	Entity& player = myPlayer->GetEntity();

	//	if (player.GetAlive() == true
	//		&& CU::Intersection::SphereVsSphere(myPlayer->GetSphere(), aComponent->GetSphere()) == true)
	//	{
	//		entity.SendMessage()
	//	}
	//}

	//for (int i = myPlayerBullets.Size() - 1; i >= 0; --i)
	//{
	//	for (int e = myEnemies.Size() - 1; e >= 0; --e)
	//	{
	//		Entity& enemy = myEnemies[e]->GetEntity();
	//		if (enemy.GetAlive() == true
	//			&& CU::Intersection::SphereVsSphere(myPlayerBullets[i]->GetSphere(), myEnemies[e]->GetSphere()) == true)
	//		{
	//			Entity& bullet = myPlayerBullets[i]->GetEntity();
	//			enemy.GetComponent<HealthComponent>()->RemoveHealth(bullet.GetComponent<BulletComponent>()->GetDamage());
	//			bullet.GetComponent<BulletComponent>()->SetActive(false);
	//			myPlayerBullets.RemoveCyclicAtIndex(i);
	//			break;
	//		}
	//	}
	//}
}