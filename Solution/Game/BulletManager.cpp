#include "stdafx.h"
#include "BulletManager.h"
#include <Camera.h>
#include <Instance.h>
#include "PostMaster.h"
#include <XMLReader.h>
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "BulletComponent.h"
#include <Engine.h>
#include <FileWatcher.h>

BulletManager::BulletManager()
{
	myInstances.Init(8);
	myBoxBulletData = nullptr;
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	// move to weapon factory:
	
	ReadFromXML("Data/script/weapon.xml");
	//for (; bulletElement != nullptr; bulletElement = reader.FindNextElement(bulletElement))
	WATCH_FILE("Data/script/weapon.xml", BulletManager::ReadFromXML);
}

BulletManager::~BulletManager()
{
	if (myBoxBulletData != nullptr)
	{
		myBoxBulletData->myBullets.DeleteAll();
		delete myBoxBulletData;
		myBoxBulletData = nullptr;
	}
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < myBoxBulletData->myMaxBullet; i++)
	{
		if (myBoxBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myBoxBulletData->myBullets[i]->Update(aDeltaTime);
		}
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	if (aMessage.GetBulletType() == eBulletType::BOX_BULLET)
	{
		ActivateBoxBullet(aMessage.GetOrientation());
	}
}

void BulletManager::ReadFromXML(const std::string aFilePath)
{
	XMLReader reader;
	reader.OpenDocument(aFilePath);
	tinyxml2::XMLElement* bulletElement = reader.FindFirstChild("weapon");
	bulletElement = reader.FindFirstChild(bulletElement, "projectile");

	if (myBoxBulletData != nullptr)
	{
		myBoxBulletData->myBullets.DeleteAll();
		delete myBoxBulletData;
		myBoxBulletData = nullptr;
	}
	BulletData* bulletData = new BulletData;

	std::string type;
	CU::Vector3<float> size;
	float totalLife = 0.f;
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "type"), "value", type);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "maxAmount"), "value", bulletData->myMaxBullet);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "size"), "x", size.x);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "size"), "y", size.y);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "size"), "z", size.z);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "lifeTime"), "value", totalLife);
	reader.ReadAttribute(reader.FindFirstChild(bulletElement, "speed"), "value", bulletData->mySpeed);

	bulletData->myBulletCounter = 0;
	bulletData->myBullets.Init(bulletData->myMaxBullet);

	for (int i = 0; i < bulletData->myMaxBullet; i++)
	{
		Entity* newEntity = new Entity();
		newEntity->AddComponent<GraphicsComponent>()->InitCube(size.x, size.y, size.z);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
		newEntity->AddComponent<PhysicsComponent>();
		newEntity->AddComponent<BulletComponent>()->Init(totalLife);
		bulletData->myBullets.Add(newEntity);
	}

	if (type == "box")
	{
		bulletData->myType = eBulletType::BOX_BULLET;
		myBoxBulletData = bulletData;
	}
}

void BulletManager::ActivateBoxBullet(const CU::Matrix44<float>& anOrientation)
{
	// has a limited amount of bullets, re-uses old after they've run out
	myBoxBulletData->myBullets[myBoxBulletData->myBulletCounter]->GetComponent<PhysicsComponent>()->Init(anOrientation, 
		anOrientation.GetForward() * myBoxBulletData->mySpeed);
	myBoxBulletData->myBullets[myBoxBulletData->myBulletCounter]->GetComponent<BulletComponent>()->SetIsActive(true);

	myBoxBulletData->myBulletCounter++;

	if (myBoxBulletData->myBulletCounter >= myBoxBulletData->myMaxBullet)
	{
		myBoxBulletData->myBulletCounter = 0;
	}
}

CU::GrowingArray<Prism::Instance*>& BulletManager::GetInstances()
{
	myInstances.RemoveAll();

	for (int i = 0; i < myBoxBulletData->myMaxBullet; i++)
	{
		if (myBoxBulletData->myBullets[i]->GetComponent<BulletComponent>()->GetIActive() == true)
		{
			myInstances.Add(myBoxBulletData->myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance());
		}
	}

	return myInstances;
}