#include "stdafx.h"
#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletComponent.h"
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Enums.h"
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include <Instance.h>
#include "PostMaster.h"
#include "PhysicsComponent.h"
#include "WeaponFactory.h"
#include <XMLReader.h>
#include "ShootingComponent.h"

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
		, aMessage.GetEntityType(), aMessage.GetEntityVelocity(), aMessage.GetIsHoming());
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
	tinyxml2::XMLElement* projectileElement;
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");
	if (rootElement == nullptr) 
	{
		projectileElement = rootDocument.FindFirstChild("Projectile");
	}
	else
	{
		projectileElement = rootDocument.FindFirstChild(rootElement, "Projectile");
	}
	
	rootDocument.ForceReadAttribute(projectileElement, "type", projectileLoaderType);
	rootDocument.CloseDocument();

	BulletData* bulletData = new BulletData;
	ProjectileDataType projectileDataType = aWeaponFactory->GetProjectile(projectileLoaderType);


	bulletData->myMaxBullet = projectileDataType.myMaxBullet;
	bulletData->mySpeed = projectileDataType.mySpeed;

	bulletData->myPlayerBulletCounter = 0;
	bulletData->myPlayerBullets.Init(bulletData->myMaxBullet);
	bulletData->myPlayerBulletComponents.Init(bulletData->myMaxBullet);

	for (int i = 0; i < bulletData->myMaxBullet; i++)
	{
		Entity* newEntity = new Entity(eEntityType::PLAYER_BULLET, myScene, Prism::eOctreeType::NOT_IN_OCTREE);
		aEntityFactory->CopyEntity(newEntity, projectileDataType.myEntityType);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
		
		bulletData->myPlayerBulletComponents.Add(newEntity->GetComponent<BulletComponent>());
		bulletData->myPlayerBullets.Add(newEntity);
	}

	bulletData->myEnemyBulletCounter = 0;
	bulletData->myEnemyBullets.Init(bulletData->myMaxBullet);
	bulletData->myEnemyBulletComponents.Init(bulletData->myMaxBullet);
	for (int i = 0; i < bulletData->myMaxBullet; i++)
	{
		Entity* newEntity = new Entity(eEntityType::ENEMY_BULLET, myScene, Prism::eOctreeType::NOT_IN_OCTREE);
		aEntityFactory->CopyEntity(newEntity, projectileDataType.myEntityType);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });

		bulletData->myEnemyBulletComponents.Add(newEntity->GetComponent<BulletComponent>());
		bulletData->myEnemyBullets.Add(newEntity);
	}

	bulletData->myType = ConvertToBulletEnum(projectileDataType.myType);

	if (bulletData->myType == eBulletType::COUNT)
	{
		std::string errorMessage = "[BulletManager] No bullet with name " + projectileDataType.myType;
		DL_ASSERT(errorMessage.c_str());
	}

	DeleteWeaponData(myBulletDatas[static_cast<int>(bulletData->myType)]);
	myBulletDatas[static_cast<int>(bulletData->myType)] = bulletData;
}

void BulletManager::ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation
	, eEntityType aEntityType, const CU::Vector3<float>& aEnitityVelocity, bool aIsHoming)
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

	if (aEntityType == eEntityType::PLAYER)
	{
		if (aWeaponData->myType == eBulletType::MACHINGUN_BULLET_LEVEL_1
			|| aWeaponData->myType == eBulletType::MACHINGUN_BULLET_LEVEL_2
			|| aWeaponData->myType == eBulletType::MACHINGUN_BULLET_LEVEL_3)
		{
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Laser", bullet->GetAudioSFXID());
		}
		if (aWeaponData->myType == eBulletType::SHOTGUN_BULLET_LEVEL_1
			|| aWeaponData->myType == eBulletType::SHOTGUN_BULLET_LEVEL_2
			|| aWeaponData->myType == eBulletType::SHOTGUN_BULLET_LEVEL_3)
		{
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Shotgun", bullet->GetAudioSFXID());
		}
		if (aWeaponData->myType == eBulletType::ROCKET_MISSILE_LEVEL_1
			|| aWeaponData->myType == eBulletType::ROCKET_MISSILE_LEVEL_2
			|| aWeaponData->myType == eBulletType::ROCKET_MISSILE_LEVEL_3)
		{
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_Rocket", bullet->GetAudioSFXID());
		}
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

	if (bullet->GetComponent<AIComponent>() != nullptr)
	{
		bullet->RemoveComponent<AIComponent>();
	}

	bullet->GetComponent<PhysicsComponent>()->Init(anOrientation,
		(anOrientation.GetForward() * (aWeaponData->mySpeed)) + aEnitityVelocity);
	bullet->GetComponent<BulletComponent>()->SetActive(true);
	bullet->GetComponent<CollisionComponent>()->Update(0.5f);

	if (aIsHoming == true)
	{
		Entity* enemy = myCollisionManager.GetClosestEnemyWithinSphere(anOrientation.GetPos(), 2000.f); // replace this target lock
		if (enemy != nullptr)
		{
			bullet->AddComponent<AIComponent>()->Init(CU::Length((anOrientation.GetForward() * (aWeaponData->mySpeed)) + aEnitityVelocity) / 10.f, 
				eAITargetPositionMode::KAMIKAZE);
			bullet->GetComponent<AIComponent>()->SetEntityToFollow(enemy);
		}
	}

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
	BulletComponent* playerBulletComp = nullptr;
	BulletComponent* enemyBulletComp = nullptr;

	for (int i = 0; i < aWeaponData->myMaxBullet; ++i)
	{
		playerBulletComp = aWeaponData->myPlayerBulletComponents[i];
		if (playerBulletComp->GetActive() == true)
		{
			aWeaponData->myPlayerBullets[i]->Update(aDeltaTime);

			if (playerBulletComp->GetActive() == false)
			{
				if (playerBulletComp->GetEntity().GetComponent<AIComponent>() != nullptr)
				{
					playerBulletComp->GetEntity().RemoveComponent<AIComponent>();
				}
				myCollisionManager.Remove(aWeaponData->myPlayerBullets[i]->GetComponent<CollisionComponent>()
					, eEntityType::PLAYER_BULLET);
			}
		}

		enemyBulletComp = aWeaponData->myEnemyBulletComponents[i];
		if (enemyBulletComp->GetActive() == true)
		{
			aWeaponData->myEnemyBullets[i]->Update(aDeltaTime);

			if (enemyBulletComp->GetActive() == false)
			{
				if (enemyBulletComp->GetEntity().GetComponent<AIComponent>() != nullptr)
				{
					enemyBulletComp->GetEntity().RemoveComponent<AIComponent>();
				}
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
				if (myBulletDatas[i]->myPlayerBulletComponents[j]->GetActive() == true)
				{
					myInstances.Add(myBulletDatas[i]->myPlayerBullets[j]->GetComponent<GraphicsComponent>()->GetInstance());
				}

				if (myBulletDatas[i]->myEnemyBulletComponents[j]->GetActive() == true)
				{
					myInstances.Add(myBulletDatas[i]->myEnemyBullets[j]->GetComponent<GraphicsComponent>()->GetInstance());
				}
			}
		}
	}
	return myInstances;
}