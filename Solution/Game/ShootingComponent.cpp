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

ShootingComponent::ShootingComponent()
{
	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", ShootingComponent::ReadFromXML);
}

ShootingComponent::~ShootingComponent()
{
	delete myCurrentWeapon;
	myCurrentWeapon = nullptr;
}

void ShootingComponent::Update(float aDeltaTime)
{
	if (myCurrentWeapon->myCurrentTime >= myCurrentWeapon->myCoolDownTime)
	{
		myCurrentWeapon->myCurrentTime = myCurrentWeapon->myCoolDownTime;
	}
	else
	{
		myCurrentWeapon->myCurrentTime += aDeltaTime;
	}
}

void ShootingComponent::ReceiveMessage(const ShootMessage&)
{
	if (myCurrentWeapon->myCurrentTime == myCurrentWeapon->myCoolDownTime)
	{
		CU::Matrix44<float> orientation = myEntity->myOrientation;
		orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f));
		PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, orientation));
		myCurrentWeapon->myCurrentTime = 0.f;
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
	tinyxml2::XMLElement* weaponElement = reader.FindFirstChild("weapon");

	if (myCurrentWeapon != nullptr)
	{
		delete myCurrentWeapon;

		myCurrentWeapon = nullptr;
	}

	WeaponData* weaponData = new WeaponData;

	std::string type;
	reader.ReadAttribute(reader.FindFirstChild(weaponElement, "type"), "value", type);
	reader.ReadAttribute(reader.FindFirstChild(weaponElement, "cooldown"), "value", weaponData->myCoolDownTime);
	weaponData->myCurrentTime = weaponData->myCoolDownTime;

	if (type == "testGun")
	{
		weaponData->myBulletType = eBulletType::BOX_BULLET;
		myCurrentWeapon = weaponData;
	}
}