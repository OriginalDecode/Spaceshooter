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
	class Scene;
}

namespace tinyxml2
{
	class XMLElement;
}

class CollisionManager;
class EntityFactory;
class WeaponFactory;

class BulletManager : public Subscriber
{
public:
	BulletManager(CollisionManager& aCollisionManager, Prism::Scene& aScene);
	~BulletManager();


	void LoadFromFactory(WeaponFactory* aWeaponFactory, EntityFactory* aEntityFactory, 
			const std::string& aProjectileList);

	void Update(float aDeltaTime);
	void ReceiveMessage(const BulletMessage& aMessage) override;

	CU::GrowingArray<Prism::Instance*>& GetInstances();

private:
	void operator=(BulletManager&) = delete;

	void LoadProjectile(WeaponFactory* aWeaponFactory, EntityFactory* aEntityFactory, const std::string& aProjectilePath);

	void ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation, eEntityType aEntityType);
	void UpdateBullet(BulletData* aWeaponData, const float& aDeltaTime);

	void DeleteWeaponData(BulletData* aWeaponData);

	CU::StaticArray<BulletData*, static_cast<int>(eBulletType::COUNT)> myBulletDatas;

	CollisionManager& myCollisionManager;
	Prism::Scene& myScene;

	// temp!!
	CU::GrowingArray<Prism::Instance*> myInstances;

};