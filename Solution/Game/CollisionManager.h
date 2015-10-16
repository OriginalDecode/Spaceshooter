#pragma once

#include "Enums.h"
#include <GrowingArray.h>
#include "Subscriber.h"

class CollisionComponent;

class CollisionManager : public Subscriber
{
public:

	CollisionManager();
	~CollisionManager();

	void Add(CollisionComponent* aComponent, eEntityType aEnum);
	void Remove(CollisionComponent* aComponent, eEntityType aEnum);

	void Update();
	void CleanUp();

	void ReceiveMessage(const PowerUpMessage& aMessage) override;

	int GetEnemiesAlive() const;

	void DisableEnemiesWithinSphere(CU::Vector3<float> aCenter, float aRadius, float aTime);
	void DamageEnemiesWithinSphere(CU::Vector3<float> aCenter, float aRadius, int aDamage);

private:
	void CheckAllCollisions(CollisionComponent* aComponent, int aFilter);
	void CheckCollision(CollisionComponent* aComponent, CU::GrowingArray<CollisionComponent*>& someOtherComponents);

	int myPlayerFilter;
	int myEnemyFilter;
	int myPlayerBulletFilter;
	int myEnemyBulletFilter;
	int myTriggerFilter;
	int myPropFilter;
	int myPowerUpFilter;

	CU::GrowingArray<CollisionComponent*> myPlayers;
	CU::GrowingArray<CollisionComponent*> myEnemies;
	CU::GrowingArray<CollisionComponent*> myPlayerBullets;
	CU::GrowingArray<CollisionComponent*> myEnemyBullets;
	CU::GrowingArray<CollisionComponent*> myTriggers;
	CU::GrowingArray<CollisionComponent*> myPowerUps;
	CU::GrowingArray<CollisionComponent*> myProps;
};

inline int CollisionManager::GetEnemiesAlive() const
{
	return myEnemies.Size();
}