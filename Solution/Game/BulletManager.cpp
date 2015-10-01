#include "stdafx.h"
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionManager.h"
#include <Instance.h>
#include "PostMaster.h"
#include <XMLReader.h>
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "BulletComponent.h"
#include "CollisionComponent.h"
#include <Engine.h>
#include <FileWatcher.h>

BulletManager::BulletManager()
	: myInstances(8)
	, myCollisionManager(nullptr)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", BulletManager::ReadFromXML);
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		DeleteWeaponData(myBulletDatas[i]);
	}
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		UpdateBullet(myBulletDatas[i], aDeltaTime);
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	ActivateBullet(myBulletDatas[static_cast<int>(aMessage.GetBulletType())], aMessage.GetOrientation()
		, aMessage.GetEntityType());
}

void BulletManager::ReadFromXML(const std::string aFilePath)
{
	XMLReader reader;
	reader.OpenDocument(aFilePath);

	tinyxml2::XMLElement* weaponElement = reader.FindFirstChild("weapon");
	for (; weaponElement != nullptr; weaponElement = reader.FindNextElement(weaponElement))
	{
		tinyxml2::XMLElement* bulletElement = reader.FindFirstChild(weaponElement, "projectile");

		BulletData* bulletData = new BulletData;

		std::string type;
		std::string modelPath;
		std::string shaderPath;
		float totalLife = 0.f;
		float sphereRadius = 0.f;
		int damage = 0;
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "model"), "path", modelPath);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "shader"), "path", shaderPath);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "type"), "value", type);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "maxAmount"), "value", bulletData->myMaxBullet);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "lifeTime"), "value", totalLife);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "speed"), "value", bulletData->mySpeed);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "damage"), "value", damage);
		reader.ReadAttribute(reader.FindFirstChild(bulletElement, "sphere"), "radius", sphereRadius);

		bulletData->myPlayerBulletCounter = 0;
		bulletData->myPlayerBullets.Init(bulletData->myMaxBullet);

		for (int i = 0; i < bulletData->myMaxBullet; i++)
		{
			Entity* newEntity = new Entity(eEntityType::PLAYER_BULLET);
			newEntity->AddComponent<GraphicsComponent>()->Init(modelPath.c_str(), shaderPath.c_str());
			newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
			newEntity->AddComponent<PhysicsComponent>();
			newEntity->AddComponent<BulletComponent>()->Init(totalLife, static_cast<unsigned short>(damage));
			newEntity->AddComponent<CollisionComponent>()->Initiate(sphereRadius);
			bulletData->myPlayerBullets.Add(newEntity);
		}


		bulletData->myEnemyBulletCounter = 0;
		bulletData->myEnemyBullets.Init(bulletData->myMaxBullet);
		for (int i = 0; i < bulletData->myMaxBullet; i++)
		{
			Entity* newEntity = new Entity(eEntityType::ENEMY_BULLET);
			newEntity->AddComponent<GraphicsComponent>()->Init(modelPath.c_str(), shaderPath.c_str());
			newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
			newEntity->AddComponent<PhysicsComponent>();
			newEntity->AddComponent<BulletComponent>()->Init(totalLife, static_cast<unsigned short>(damage));
			newEntity->AddComponent<CollisionComponent>()->Initiate(sphereRadius);
			bulletData->myEnemyBullets.Add(newEntity);
		}



		if (type == "machinegun")
		{
			bulletData->myType = eBulletType::MACHINGUN_BULLET;
		}
		else if (type == "sniper")
		{
			bulletData->myType = eBulletType::SNIPER_BULLET;
		}
		else if (type == "plasma")
		{
			bulletData->myType = eBulletType::PLASMA_BULLET;
		}

		DeleteWeaponData(myBulletDatas[static_cast<int>(bulletData->myType)]);
		myBulletDatas[static_cast<int>(bulletData->myType)] = bulletData;
	}
}

void BulletManager::ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation
	, eEntityType aEntityType)
{
	DL_ASSERT_EXP(myCollisionManager != nullptr, "Tried to Activate Bullet without a Collisionmanager");

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
			myCollisionManager->Add(bullet->GetComponent<CollisionComponent>(), eEntityType::PLAYER_BULLET);
		}
		else if (aEntityType == eEntityType::ENEMY)
		{
			myCollisionManager->Add(bullet->GetComponent<CollisionComponent>(), eEntityType::ENEMY_BULLET);
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
				myCollisionManager->Remove(aWeaponData->myPlayerBullets[i]->GetComponent<CollisionComponent>()
					, eEntityType::PLAYER_BULLET);
			}
		}

		if (aWeaponData->myEnemyBullets[i]->GetComponent<BulletComponent>()->GetActive() == true)
		{
			aWeaponData->myEnemyBullets[i]->Update(aDeltaTime);

			if (aWeaponData->myEnemyBullets[i]->GetComponent<BulletComponent>()->GetActive() == false)
			{
				myCollisionManager->Remove(aWeaponData->myEnemyBullets[i]->GetComponent<CollisionComponent>()
					, eEntityType::ENEMY_BULLET);
			}
		}
	}
}

void BulletManager::DeleteWeaponData(BulletData* aWeaponData)
{
	if (aWeaponData != nullptr)
	{
		for (int i = 0; i < aWeaponData->myPlayerBullets.Size(); ++i)
		{
			delete aWeaponData->myPlayerBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}
		aWeaponData->myPlayerBullets.DeleteAll();

		for (int i = 0; i < aWeaponData->myEnemyBullets.Size(); ++i)
		{
			delete aWeaponData->myEnemyBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}
		aWeaponData->myEnemyBullets.DeleteAll();

		delete aWeaponData;
		aWeaponData = nullptr;
	}
}

CU::GrowingArray<Prism::Instance*>& BulletManager::GetInstances()
{
	myInstances.RemoveAll();

	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
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
	return myInstances;
}

void BulletManager::Reset()
{
	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		for (int j = 0; j < myBulletDatas[i]->myMaxBullet; ++j)
		{
			myBulletDatas[i]->myPlayerBullets[j]->GetComponent<BulletComponent>()->SetActive(false);
			myBulletDatas[i]->myEnemyBullets[j]->GetComponent<BulletComponent>()->SetActive(false);
		}

		myBulletDatas[i]->myPlayerBulletCounter = 0;
		myBulletDatas[i]->myEnemyBulletCounter = 0;
	}
}
