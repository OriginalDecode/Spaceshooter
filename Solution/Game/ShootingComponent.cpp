#include "stdafx.h"
#include "ShootingComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"
#include "PostMaster.h"
#include "BulletMessage.h"
#include <FileWatcher.h>
#include <XMLReader.h>

#define PI 3.14159265359f

ShootingComponent::ShootingComponent()
{
	myWeapons.Init(4);
	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", ShootingComponent::ReadFromXML);
	myCurrentWeapon = 0;
}

ShootingComponent::~ShootingComponent()
{
}

void ShootingComponent::Update(float aDeltaTime)
{
	if (myWeapons[myCurrentWeapon].myCurrentTime >= myWeapons[myCurrentWeapon].myCoolDownTime)
	{
		myWeapons[myCurrentWeapon].myCurrentTime = myWeapons[myCurrentWeapon].myCoolDownTime;
	}
	else
	{
		myWeapons[myCurrentWeapon].myCurrentTime += aDeltaTime;
	}
}

void ShootingComponent::ReceiveMessage(const ShootMessage&)
{
	if (myWeapons[myCurrentWeapon].myCurrentTime == myWeapons[myCurrentWeapon].myCoolDownTime)
	{
		CU::Matrix44<float> orientation = myEntity->myOrientation;
		orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f));

		if (myWeapons[myCurrentWeapon].mySpread > 0)
		{
			float randomSpreadX = float((rand() % (myWeapons[myCurrentWeapon].mySpread * 2)) - myWeapons[myCurrentWeapon].mySpread) / 100.f;
			float randomSpreadY = float((rand() % (myWeapons[myCurrentWeapon].mySpread * 2)) - myWeapons[myCurrentWeapon].mySpread) / 100.f;
			
			CU::Matrix44<float> rotation;
			rotation.myMatrix[8] = randomSpreadX;
			rotation.myMatrix[9] = randomSpreadY;

			CU::Vector4<float> pos = orientation.GetPos();
			orientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
			orientation = rotation * orientation;
			orientation.SetPos(pos);
		}

		PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, orientation));
		myWeapons[myCurrentWeapon].myCurrentTime = 0.f;
	}
}

void ShootingComponent::Init(CU::Vector3<float> aSpawningPointOffset)
{
	mySpawningPointOffset = aSpawningPointOffset;
}

void ShootingComponent::ReadFromXML(const std::string aFilePath)
{
	XMLReader reader;
	reader.OpenDocument(aFilePath);

	// for each weapon
	tinyxml2::XMLElement* weaponElement = reader.FindFirstChild("weapon");

	WeaponData weaponData;

	std::string type;
	reader.ReadAttribute(reader.FindFirstChild(weaponElement, "type"), "value", type);
	reader.ReadAttribute(reader.FindFirstChild(weaponElement, "cooldown"), "value", weaponData.myCoolDownTime);
	reader.ReadAttribute(reader.FindFirstChild(weaponElement, "spread"), "value", weaponData.mySpread);
	weaponData.myCurrentTime = weaponData.myCoolDownTime;

	if (type == "testGun")
	{
		weaponData.myBulletType = eBulletType::BOX_BULLET;
	}

	myWeapons.Add(weaponData);
}