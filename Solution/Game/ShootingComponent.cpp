#include "stdafx.h"
#include "BulletMessage.h"
#include "Entity.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include "InputNote.h"
#include "PostMaster.h"
#include "PhysicsComponent.h"
#include "PowerUpNote.h"
#include "ShootingComponent.h"
#include "ShootNote.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

#define PI 3.14159265359f

ShootingComponent::ShootingComponent(Entity& aEntity)
	: myWeapons(8)
	, myCurrentWeaponID(0)
	, Component(aEntity)
	, myHasWeapon(false)
{
}

void ShootingComponent::Update(float aDeltaTime)
{
	if (myHasWeapon == true)
	{
		if (myWeapons[myCurrentWeaponID].myCurrentTime >= myWeapons[myCurrentWeaponID].myCoolDownTime)
		{
			myWeapons[myCurrentWeaponID].myCurrentTime = myWeapons[myCurrentWeaponID].myCoolDownTime;
		}
		else
		{
			myWeapons[myCurrentWeaponID].myCurrentTime += myWeapons[myCurrentWeaponID].myMultiplier * aDeltaTime;
		}
	}
}

void ShootingComponent::ReceiveNote(const ShootNote& aShootNote)
{
	if (myWeapons[myCurrentWeaponID].myCurrentTime == myWeapons[myCurrentWeaponID].myCoolDownTime)
	{
		CU::Matrix44<float> orientation = myEntity.myOrientation;
		orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f)
			+ (myWeapons[myCurrentWeaponID].myPosition * myEntity.myOrientation));

		if (myWeapons[myCurrentWeaponID].mySpread > 0)
		{
			float randomSpreadX = float((rand() % (myWeapons[myCurrentWeaponID].mySpread * 2)) - myWeapons[myCurrentWeaponID].mySpread) / 100.f;
			float randomSpreadY = float((rand() % (myWeapons[myCurrentWeaponID].mySpread * 2)) - myWeapons[myCurrentWeaponID].mySpread) / 100.f;
			
			CU::Matrix44<float> rotation;
			rotation.myMatrix[8] = randomSpreadX;
			rotation.myMatrix[9] = randomSpreadY;

			CU::Vector4<float> pos = orientation.GetPos();
			orientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
			orientation = rotation * orientation;
			orientation.SetPos(pos);
		}

		PostMaster::GetInstance()->SendMessage(BulletMessage(myWeapons[myCurrentWeaponID].myBulletType, orientation
			, myEntity.GetType(), aShootNote.myEnitityVelocity));
		myWeapons[myCurrentWeaponID].myCurrentTime = 0.f;
	}
}

void ShootingComponent::ReceiveNote(const InputNote& aMessage)
{
	SetCurrentWeaponID(aMessage.myKey);
}

void ShootingComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::FIRERATEBOOST)
	{
		for (unsigned int i = 0; i < myWeapons.Size(); ++i)
		{
			myWeapons[i].myMultiplier = aNote.myFireRateMultiplier;
		}
	}
}

void ShootingComponent::AddWeapon(const WeaponDataType& aWeapon)
{
	WeaponData newWeapon;

	newWeapon.myCoolDownTime = aWeapon.myCoolDownTime;
	newWeapon.myCurrentTime = aWeapon.myCoolDownTime;
	newWeapon.myPosition = aWeapon.myPosition;
	newWeapon.mySpread = aWeapon.mySpread;
	newWeapon.myType = aWeapon.myType;
	newWeapon.myMultiplier = 1;

	if (aWeapon.myBulletType == "machinegun")
	{
		newWeapon.myBulletType = eBulletType::MACHINGUN_BULLET;
	}
	else if (aWeapon.myBulletType == "sniper")
	{
		newWeapon.myBulletType = eBulletType::SNIPER_BULLET;
	}
	else if (aWeapon.myBulletType == "plasma")
	{
		newWeapon.myBulletType = eBulletType::PLASMA_BULLET;
	}

	newWeapon.myID = static_cast<int>(newWeapon.myBulletType);
	myCurrentWeaponID = newWeapon.myID;
	myWeapons.Add(newWeapon);
	myHasWeapon = true;
}
