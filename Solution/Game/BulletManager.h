#pragma once
#include "Entity.h"
#include "BulletMessage.h"
#include "Subscriber.h"
#include "StaticArray.h"

struct BulletData
{
	CU::GrowingArray<Entity*> myPlayerBullets;
	CU::GrowingArray<Entity*> myEnemyBullets;
	eBulletType myType;
	float mySpeed;
	int myPlayerBulletCounter;
	int myEnemyBulletCounter;
	int myMaxBullet;
};


namespace Prism
{
	class Camera;
	class Instance;
}

namespace tinyxml2
{
	class XMLElement;
}

class CollisionManager;

class BulletManager : public Subscriber
{
public:
	BulletManager();
	~BulletManager();

	void ReadFromXML(const std::string aFilePath);
	void Update(float aDeltaTime);
	void ReceiveMessage(const BulletMessage& aMessage) override;

	void SetCollisionManager(CollisionManager* aCollisionManager);

	CU::GrowingArray<Prism::Instance*>& GetInstances();

	void Reset();

private:

	void ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType);
	void UpdateBullet(BulletData* aWeaponData, const float& aDeltaTime);

	void DeleteWeaponData(BulletData* aWeaponData);

	CU::StaticArray<BulletData*, static_cast<int>(eBulletType::COUNT)> myBulletDatas;

	CollisionManager* myCollisionManager;

	// temp!!
	CU::GrowingArray<Prism::Instance*> myInstances;

};

inline void BulletManager::SetCollisionManager(CollisionManager* aCollisionManager)
{
	myCollisionManager = aCollisionManager;
}