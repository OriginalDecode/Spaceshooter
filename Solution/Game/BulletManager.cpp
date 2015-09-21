#include "stdafx.h"
#include "BulletManager.h"
#include <Camera.h>
#include <Instance.h>
#include "PostMaster.h"
#include <XMLReader.h>

BulletManager::BulletManager()
{
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	// move to weapon factory:
	XMLReader reader; 
	reader.OpenDocument("Data/script/weapon.xml");
	tinyxml2::XMLElement* bulletElement = reader.FindFirstChild("weapon");
	bulletElement = reader.FindFirstChild(bulletElement, "projectile");
	ReadFromXML(reader, bulletElement);
	//for (; bulletElement != nullptr; bulletElement = reader.FindNextElement(bulletElement))
}

BulletManager::~BulletManager()
{
	myBoxBulletData.myBullets.DeleteAll();
}

void BulletManager::Update(float aDeltaTime)
{
	for (int i = 0; i < myBoxBulletData.myMaxBullet; i++)
	{
		if (myBoxBulletData.myIsActiveBullets[i] == true)
		{
			myBoxBulletData.myBullets[i]->Update(aDeltaTime);

		}
	}

	// todo: life counter on bullets
}

void BulletManager::Render(Prism::Camera* aCamera)
{
	for (int i = 0; i < myBoxBulletData.myMaxBullet; i++)
	{
		if (myBoxBulletData.myIsActiveBullets[i] == true)
		{
			myBoxBulletData.myBullets[i]->GetComponent<GraphicsComponent>()->GetInstance()->Render(*aCamera);

		}
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	if (aMessage.GetBulletType() == eBulletType::BOX_BULLET)
	{
		ActivateBoxBullet(aMessage.GetVelocity(), aMessage.GetPosition());
	}
}

void BulletManager::ReadFromXML(XMLReader& aXMLReader, tinyxml2::XMLElement* aBulletElement)
{
	BulletData bulletData;

	std::string aType;

	CU::Vector3<float> size;
	aXMLReader.ReadAttribute(aXMLReader.FindFirstChild(aBulletElement, "type"), "value", aType);
	aXMLReader.ReadAttribute(aXMLReader.FindFirstChild(aBulletElement, "maxAmount"), "value", bulletData.myMaxBullet);
	aXMLReader.ReadAttribute(aXMLReader.FindFirstChild(aBulletElement, "size"), "x", size.x);
	aXMLReader.ReadAttribute(aXMLReader.FindFirstChild(aBulletElement, "size"), "y", size.y);
	aXMLReader.ReadAttribute(aXMLReader.FindFirstChild(aBulletElement, "size"), "z", size.z);

	bulletData.myBulletCounter = 0;
	bulletData.myBullets.Init(bulletData.myMaxBullet);
	bulletData.myIsActiveBullets.Init(bulletData.myMaxBullet);

	for (int i = 0; i < bulletData.myMaxBullet; i++)
	{
		Entity* newEntity = new Entity();
		newEntity->AddComponent<GraphicsComponent>()->InitCube(size.x, size.y, size.z);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({ 0, 0, 0 });
		newEntity->AddComponent<PhysicsComponent>()->Init({ 0, 0, 0 }, { 0, 0, 0 });
		bulletData.myBullets.Add(newEntity);
		bulletData.myIsActiveBullets.Add(false);
	}

	if (aType == "box")
	{
		bulletData.myType = eBulletType::BOX_BULLET;
		myBoxBulletData = bulletData;
	}
}

void BulletManager::ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition)
{
	// has a limited amount of bullets, re-uses old after they've run out
	myBoxBulletData.myBullets[myBoxBulletData.myBulletCounter]->GetComponent<PhysicsComponent>()->Init(aVelocity, aPosition);
	myBoxBulletData.myIsActiveBullets[myBoxBulletData.myBulletCounter] = true;

	myBoxBulletData.myBulletCounter++;

	if (myBoxBulletData.myBulletCounter >= myBoxBulletData.myMaxBullet)
	{
		myBoxBulletData.myBulletCounter = 0;
	}
}