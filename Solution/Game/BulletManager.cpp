#include "stdafx.h"
#include "BulletComponent.h"
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include <Engine.h>
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include <Instance.h>
#include "PostMaster.h"
#include "PhysicsComponent.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

BulletManager::BulletManager(CollisionManager& aCollisionManager, Prism::Scene& aScene)
	: myInstances(8)
	, myCollisionManager(aCollisionManager)
	, myScene(aScene)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		myBulletDatas[i] = nullptr;
	}

	/*ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", BulletManager::ReadFromXML);*/
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		DeleteWeaponData(myBulletDatas[i]);
	}

	PostMaster::GetInstance()->UnSubscribe(eMessageType::ACTIVATE_BULLET, this);
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		if (myBulletDatas[i] != nullptr)
		{
			UpdateBullet(myBulletDatas[i], aDeltaTime);
		}
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	ActivateBullet(myBulletDatas[static_cast<int>(aMessage.GetBulletType())], aMessage.GetOrientation()
		, aMessage.GetEntityType());
}

void BulletManager::LoadFromFactory(WeaponFactory* aWeaponFactory, EntityFactory* aEntityFactory, 
		const std::string& aProjectileList)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aProjectileList);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");

	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string projectilePath = "";
		rootDocument.ForceReadAttribute(e, "src", projectilePath);
		if (projectilePath != "")
		{
			LoadProjectile(aWeaponFactory, aEntityFactory, projectilePath);
		}
	}

	rootDocument.CloseDocument();
}

void BulletManager::LoadProjectile(WeaponFactory* aWeaponFactory, EntityFactory* aEntityFactory, 
		const std::string& aProjectilePath)
{
	std::string projectileLoaderType;
	XMLReader rootDocument;
	rootDocument.OpenDocument(aProjectilePath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("Projectile");
	rootDocument.ForceReadAttribute(rootElement, "type", projectileLoaderType);
	rootDocument.CloseDocument();

	BulletData* bulletData = new BulletData;
	ProjectileDataType projectileDataType = aWeaponFactory->GetProjectile(projectileLoaderType);


	bulletData->myMaxBullet = projectileDataType.myMaxBullet;
	bulletData->mySpeed = projectileDataType.mySpeed;

	bulletData->myPlayerBulletCounter = 0;
	bulletData->myPlayerBullets.Init(bulletData->myMaxBullet);

	for (int i = 0; i < bulletData->myMaxBullet; i++)
	{
		Entity* newEntity = new Entity(eEntityType::PLAYER_BULLET, myScene);
		aEntityFactory->CopyEntity(newEntity, projectileDataType.myEntityType);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
		bulletData->myPlayerBullets.Add(newEntity);
	}

	bulletData->myEnemyBulletCounter = 0;
	bulletData->myEnemyBullets.Init(bulletData->myMaxBullet);
	for (int i = 0; i < bulletData->myMaxBullet; i++)
	{
		Entity* newEntity = new Entity(eEntityType::ENEMY_BULLET, myScene);
		aEntityFactory->CopyEntity(newEntity, projectileDataType.myEntityType);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
		bulletData->myEnemyBullets.Add(newEntity);
	}

	if (projectileDataType.myType == "machinegun")
	{
		bulletData->myType = eBulletType::MACHINGUN_BULLET;
	}
	else if (projectileDataType.myType == "sniper")
	{
		bulletData->myType = eBulletType::SNIPER_BULLET;
	}
	else if (projectileDataType.myType == "plasma")
	{
		bulletData->myType = eBulletType::PLASMA_BULLET;
	}
	DeleteWeaponData(myBulletDatas[static_cast<int>(bulletData->myType)]);
	myBulletDatas[static_cast<int>(bulletData->myType)] = bulletData;
}

void BulletManager::ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation
	, eEntityType aEntityType)
{
	Entity* bullet = nullptr;
	if (aEntityType == eEntityType::PLAYER)
	{
		bullet = aWeaponData->myPlayerBullets[aWeaponData->myPlayerBulletCounter];
	}
	else if (aEntityType == eEntityType::ENEMY)
	{
		bullet = aWeaponData->myEnemyBullets[aWeaponData->myEnemyBulletCounter];
	}

	DL_ASSERT_EXP(bullet != nullptr, "Non Player/Enemy cant activate bullets!");

	if (bullet->GetComponent<BulletComponent>()->GetActive() == false)
	{
		if (aEntityType == eEntityType::PLAYER)
		{
			myCollisionManager.Add(bullet->GetComponent<CollisionComponent>(), eEntityType::PLAYER_BULLET);
		}
		else if (aEntityType == eEntityType::ENEMY)
		{
			myCollisionManager.Add(bullet->GetComponent<CollisionComponent>(), eEntityType::ENEMY_BULLET);
		}
	}


	bullet->GetComponent<PhysicsComponent>()->Init(anOrientation,
		anOrientation.GetForward() * aWeaponData->mySpeed);
	bullet->GetComponent<BulletComponent>()->SetActive(true);
	bullet->GetComponent<CollisionComponent>()->Update(0.5f);

	

	if (aEntityType == eEntityType::PLAYER)
	{
		aWeaponData->myPlayerBulletCounter++;
		if (aWeaponData->myPlayerBulletCounter >= aWeaponData->myMaxBullet)
		{
			aWeaponData->myPlayerBulletCounter = 0;
		}
	}
	else if (aEntityType == eEntityType::ENEMY)
	{
		aWeaponData->myEnemyBulletCounter++;
		if (aWeaponData->myEnemyBulletCounter >= aWeaponData->myMaxBullet)
		{
			aWeaponData->myEnemyBulletCounter = 0;
		}
	}
}

void BulletManager::UpdateBullet(BulletData* aWeaponData, const float& aDeltaTime)
{
	for (int i = 0; i < aWeaponData->myMaxBullet; ++i)
	{
		if (aWeaponData->myPlayerBullets[i]->GetComponent<BulletComponent>()->GetActive() == true)
		{
			aWeaponData->myPlayerBullets[i]->Update(aDeltaTime);

			if (aWeaponData->myPlayerBullets[i]->GetComponent<BulletComponent>()->GetActive() == false)
			{
				myCollisionManager.Remove(aWeaponData->myPlayerBullets[i]->GetComponent<CollisionComponent>()
					, eEntityType::PLAYER_BULLET);
			}
		}

		if (aWeaponData->myEnemyBullets[i]->GetComponent<BulletComponent>()->GetActive() == true)
		{
			aWeaponData->myEnemyBullets[i]->Update(aDeltaTime);

			if (aWeaponData->myEnemyBullets[i]->GetComponent<BulletComponent>()->GetActive() == false)
			{
				myCollisionManager.Remove(aWeaponData->myEnemyBullets[i]->GetComponent<CollisionComponent>()
					, eEntityType::ENEMY_BULLET);
			}
		}
	}
}

void BulletManager::DeleteWeaponData(BulletData* aWeaponData)
{
	if (aWeaponData != nullptr)
	{
		aWeaponData->myPlayerBullets.DeleteAll();
		aWeaponData->myEnemyBullets.DeleteAll();
	}

	delete aWeaponData;
	aWeaponData = nullptr;
}

CU::GrowingArray<Prism::Instance*>& BulletManager::GetInstances()
{
	myInstances.RemoveAll();

	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		if (myBulletDatas[i] != nullptr)
		{
			for (int j = 0; j < myBulletDatas[i]->myMaxBullet; ++j)
			{
				if (myBulletDatas[i]->myPlayerBullets[j]->GetComponent<BulletComponent>()->GetActive() == true)
				{
					myInstances.Add(myBulletDatas[i]->myPlayerBullets[j]->GetComponent<GraphicsComponent>()->GetInstance());
				}

				if (myBulletDatas[i]->myEnemyBullets[j]->GetComponent<BulletComponent>()->GetActive() == true)
				{
					myInstances.Add(myBulletDatas[i]->myEnemyBullets[j]->GetComponent<GraphicsComponent>()->GetInstance());
				}
			}
		}
	}
	return myInstances;
}