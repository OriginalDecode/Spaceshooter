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
{
	myInstances.Init(8);
	myMachinegunBulletData = nullptr;
	mySniperBulletData = nullptr;
	myPlasmaBulletData = nullptr;
	myCollisionManager = nullptr;
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", BulletManager::ReadFromXML);
}

BulletManager::~BulletManager()
{
	if (myMachinegunBulletData != nullptr)
	{
		for (int i = 0; i < myMachinegunBulletData->myBullets.Size(); i++)
		{
			delete myMachinegunBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}

		myMachinegunBulletData->myBullets.DeleteAll();
		delete myMachinegunBulletData;
		myMachinegunBulletData = nullptr;
	}

	if (mySniperBulletData != nullptr)
	{
		for (int i = 0; i < mySniperBulletData->myBullets.Size(); i++)
		{
			delete mySniperBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}

		mySniperBulletData->myBullets.DeleteAll();
		delete mySniperBulletData;
		mySniperBulletData = nullptr;
	}

	if (myPlasmaBulletData != nullptr)
	{
		for (int i = 0; i < myPlasmaBulletData->myBullets.Size(); i++)
		{
			delete myPlasmaBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
		}

		myPlasmaBulletData->myBullets.DeleteAll();
		delete myPlasmaBulletData;
		myPlasmaBulletData = nullptr;
	}
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < myMachinegunBulletData->myMaxBullet; i++)
	{
		if (myMachinegunBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myMachinegunBulletData->myBullets[i]->Update(aDeltaTime);
		}
	}

	for (int i = 0; i < mySniperBulletData->myMaxBullet; i++)
	{
		if (mySniperBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			mySniperBulletData->myBullets[i]->Update(aDeltaTime);
		}
	}

	for (int i = 0; i < myPlasmaBulletData->myMaxBullet; i++)
	{
		if (myPlasmaBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myPlasmaBulletData->myBullets[i]->Update(aDeltaTime);
		}
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	if (aMessage.GetBulletType() == eBulletType::MACHINGUN_BULLET)
	{
		ActivateMachinegunBullet(aMessage.GetOrientation());
	}
	else if (aMessage.GetBulletType() == eBulletType::SNIPER_BULLET)
	{
		ActivateSniperBullet(aMessage.GetOrientation());
	}
	else if (aMessage.GetBulletType() == eBulletType::PLASMA_BULLET)
	{
		ActivatePlasmaBullet(aMessage.GetOrientation());
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
			Entity* newEntity = new Entity();
			newEntity->AddComponent<GraphicsComponent>()->Init(modelPath.c_str(), shaderPath.c_str());
			newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
			newEntity->AddComponent<PhysicsComponent>();
			newEntity->AddComponent<BulletComponent>()->Init(totalLife, static_cast<unsigned short>(damage));
			newEntity->AddComponent<CollisionComponent>()->Initiate(sphereRadius);
			bulletData->myBullets.Add(newEntity);
		}

		if (type == "machinegun")
		{
			if (myMachinegunBulletData != nullptr)
			{
				for (int i = 0; i < myMachinegunBulletData->myBullets.Size(); i++)
				{
					delete myMachinegunBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
				}

				myMachinegunBulletData->myBullets.DeleteAll();
				delete myMachinegunBulletData;
				myMachinegunBulletData = nullptr;
			}

			bulletData->myType = eBulletType::MACHINGUN_BULLET;
			myMachinegunBulletData = bulletData;
		}
		else if (type == "sniper")
		{
			if (mySniperBulletData != nullptr)
			{
				for (int i = 0; i < mySniperBulletData->myBullets.Size(); i++)
				{
					delete mySniperBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
				}

				mySniperBulletData->myBullets.DeleteAll();
				delete mySniperBulletData;
				mySniperBulletData = nullptr;
			}

			bulletData->myType = eBulletType::SNIPER_BULLET;
			mySniperBulletData = bulletData;
		}
		else if (type == "plasma")
		{
			if (myPlasmaBulletData != nullptr)
			{
				for (int i = 0; i < myPlasmaBulletData->myBullets.Size(); i++)
				{
					delete myPlasmaBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance();
				}

				myPlasmaBulletData->myBullets.DeleteAll();
				delete myPlasmaBulletData;
				myPlasmaBulletData = nullptr;
			}

			bulletData->myType = eBulletType::PLASMA_BULLET;
			myPlasmaBulletData = bulletData;
		}
	}
}

void BulletManager::ActivateMachinegunBullet(const CU::Matrix44<float>& anOrientation)
{
	DL_ASSERT_EXP(myCollisionManager != nullptr, "Tried to Activate Bullet without a Collisionmanager");

	Entity& bullet = *myMachinegunBulletData->myBullets[myMachinegunBulletData->myBulletCounter];

	bullet.GetComponent<PhysicsComponent>()->Init(anOrientation,
		anOrientation.GetForward() * myMachinegunBulletData->mySpeed);
	bullet.GetComponent<BulletComponent>()->SetIsActive(true);

	myCollisionManager->Add(bullet.GetComponent<CollisionComponent>(), CollisionManager::PLAYER_BULLET);

	myMachinegunBulletData->myBulletCounter++;

	if (myMachinegunBulletData->myBulletCounter >= myMachinegunBulletData->myMaxBullet)
	{
		myMachinegunBulletData->myBulletCounter = 0;
	}
}

void BulletManager::ActivateSniperBullet(const CU::Matrix44<float>& anOrientation)
{
	mySniperBulletData->myBullets[mySniperBulletData->myBulletCounter]->GetComponent<PhysicsComponent>()->Init(anOrientation,
		anOrientation.GetForward() * mySniperBulletData->mySpeed);
	mySniperBulletData->myBullets[mySniperBulletData->myBulletCounter]->GetComponent<BulletComponent>()->SetIsActive(true);

	mySniperBulletData->myBulletCounter++;

	if (mySniperBulletData->myBulletCounter >= mySniperBulletData->myMaxBullet)
	{
		mySniperBulletData->myBulletCounter = 0;
	}
}

void BulletManager::ActivatePlasmaBullet(const CU::Matrix44<float>& anOrientation)
{
	myPlasmaBulletData->myBullets[myPlasmaBulletData->myBulletCounter]->GetComponent<PhysicsComponent>()->Init(anOrientation,
		anOrientation.GetForward() * myPlasmaBulletData->mySpeed);
	myPlasmaBulletData->myBullets[myPlasmaBulletData->myBulletCounter]->GetComponent<BulletComponent>()->SetIsActive(true);

	myPlasmaBulletData->myBulletCounter++;

	if (myPlasmaBulletData->myBulletCounter >= myPlasmaBulletData->myMaxBullet)
	{
		myPlasmaBulletData->myBulletCounter = 0;
	}
}

CU::GrowingArray<Prism::Instance*>& BulletManager::GetInstances()
{
	myInstances.RemoveAll();

	for (int i = 0; i < myMachinegunBulletData->myMaxBullet; i++)
	{
		if (myMachinegunBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myInstances.Add(myMachinegunBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance());
		}
	}

	for (int i = 0; i < mySniperBulletData->myMaxBullet; i++)
	{
		if (mySniperBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myInstances.Add(mySniperBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance());
		}
	}

	for (int i = 0; i < myPlasmaBulletData->myMaxBullet; i++)
	{
		if (myPlasmaBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myInstances.Add(myPlasmaBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance());
		}
	}

	return myInstances;
}
