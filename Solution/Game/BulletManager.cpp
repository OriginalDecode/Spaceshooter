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
	, myBulletDatas(8)
	, myMachinegunBulletData(nullptr)
	, mySniperBulletData(nullptr)
	, myPlasmaBulletData(nullptr)
	, myCollisionManager(nullptr)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", BulletManager::ReadFromXML);
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < myBulletDatas.Size(); ++i)
	{
		DeleteWeaponData(myBulletDatas[i]);
	}
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < myBulletDatas.Size(); ++i)
	{
		UpdateBullet(myBulletDatas[i], aDeltaTime);
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	if (aMessage.GetBulletType() == eBulletType::MACHINGUN_BULLET)
	{
		ActivateBullet(myMachinegunBulletData, aMessage.GetOrientation());
	}
	else if (aMessage.GetBulletType() == eBulletType::SNIPER_BULLET)
	{
		ActivateBullet(mySniperBulletData, aMessage.GetOrientation());
	}
	else if (aMessage.GetBulletType() == eBulletType::PLASMA_BULLET)
	{
		ActivateBullet(myPlasmaBulletData, aMessage.GetOrientation());
	}
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

		bulletData->myBulletCounter = 0;
		bulletData->myBullets.Init(bulletData->myMaxBullet);

		for (int i = 0; i < bulletData->myMaxBullet; i++)
		{
			Entity* newEntity = new Entity(Entity::eType::PLAYER_BULLET);
			newEntity->AddComponent<GraphicsComponent>()->Init(modelPath.c_str(), shaderPath.c_str());
			newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
			newEntity->AddComponent<PhysicsComponent>();
			newEntity->AddComponent<BulletComponent>()->Init(totalLife, static_cast<unsigned short>(damage));
			newEntity->AddComponent<CollisionComponent>()->Initiate(sphereRadius);
			bulletData->myBullets.Add(newEntity);
		}

		if (type == "machinegun")
		{
			DeleteWeaponData(myMachinegunBulletData);
			bulletData->myType = eBulletType::MACHINGUN_BULLET;
			myMachinegunBulletData = bulletData;
		}
		else if (type == "sniper")
		{
			DeleteWeaponData(mySniperBulletData);
			bulletData->myType = eBulletType::SNIPER_BULLET;
			mySniperBulletData = bulletData;
		}
		else if (type == "plasma")
		{
			DeleteWeaponData(myPlasmaBulletData);
			bulletData->myType = eBulletType::PLASMA_BULLET;
			myPlasmaBulletData = bulletData;
		}
		myBulletDatas.Add(bulletData);
	}
}

void BulletManager::ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation)
{
	DL_ASSERT_EXP(myCollisionManager != nullptr, "Tried to Activate Bullet without a Collisionmanager");

	Entity& bullet = *aWeaponData->myBullets[aWeaponData->myBulletCounter];
	if (bullet.GetComponent<BulletComponent>()->GetActive() == false)
	{
		myCollisionManager->Add(bullet.GetComponent<CollisionComponent>(), CollisionManager::PLAYER_BULLET);
	}


	bullet.GetComponent<PhysicsComponent>()->Init(anOrientation,
		anOrientation.GetForward() * aWeaponData->mySpeed);
	bullet.GetComponent<BulletComponent>()->SetActive(true);
	bullet.GetComponent<CollisionComponent>()->Update(0.5f);

	

	aWeaponData->myBulletCounter++;

	if (aWeaponData->myBulletCounter >= aWeaponData->myMaxBullet)
	{
		aWeaponData->myBulletCounter = 0;
	}
}

void BulletManager::UpdateBullet(BulletData* aWeaponData, const float& aDeltaTime)
{
	for (int i = 0; i < aWeaponData->myMaxBullet; ++i)
	{
		if (aWeaponData->myBullets[i]->GetComponent<BulletComponent>()->GetActive() == true)
		{
			aWeaponData->myBullets[i]->Update(aDeltaTime);

			if (aWeaponData->myBullets[i]->GetComponent<BulletComponent>()->GetActive() == false)
			{
				myCollisionManager->Remove(aWeaponData->myBullets[i]->GetComponent<CollisionComponent>(), CollisionManager::PLAYER_BULLET);
			}
		}
	}
}

void BulletManager::DeleteWeaponData(BulletData* aWeaponData)
{
	if (aWeaponData != nullptr)
	{
		for (int i = 0; i < aWeaponData->myBullets.Size(); ++i)
		{
			delete aWeaponData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}

		aWeaponData->myBullets.DeleteAll();
		delete aWeaponData;
		aWeaponData = nullptr;
	}
}

CU::GrowingArray<Prism::Instance*>& BulletManager::GetInstances()
{
	myInstances.RemoveAll();

	for (int i = 0; i < myBulletDatas.Size(); ++i)
	{
		for (int j = 0; j < myBulletDatas[i]->myMaxBullet; ++j)
		{
			if (myBulletDatas[i]->myBullets[j]->GetComponent<BulletComponent>()->GetActive() == true)
			{
				myInstances.Add(myBulletDatas[i]->myBullets[j]->GetComponent<GraphicsComponent>()->GetInstance());
			}
		}
	}
	return myInstances;
}
