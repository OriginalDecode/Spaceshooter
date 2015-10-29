#include "stdafx.h"
#include "BulletMessage.h"
#include "Entity.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include "GUINote.h"
#include "InputNote.h"
#include <MathHelper.h>
#include "PostMaster.h"
#include "PhysicsComponent.h"
#include "PowerUpNote.h"
#include "ShootingComponent.h"
#include "ShootNote.h"
#include "PowerUpMessage.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

#define PI 3.14159265359f

ShootingComponent::ShootingComponent(Entity& aEntity)
	: myWeapons(8)
	, myCurrentWeaponID(0)
	, Component(aEntity)
	, myHasWeapon(false)
	, myPowerUpType(ePowerUpType::NO_POWERUP)
	, myPowerUpValue(0.f)
	, myPowerUpDuration(0.f)
	, myPowerUpCoolDownReducer(1.f)
	, myHomingTarget(nullptr)
{
}

void ShootingComponent::Update(float aDeltaTime)
{
	if (myHasWeapon == true)
	{
		if (myWeapons[myCurrentWeaponID].myCurrentTime >= (myWeapons[myCurrentWeaponID].myCoolDownTime))
		{
			myWeapons[myCurrentWeaponID].myCurrentTime = myWeapons[myCurrentWeaponID].myCoolDownTime;
		}
		else
		{
			myWeapons[myCurrentWeaponID].myCurrentTime += aDeltaTime * myPowerUpCoolDownReducer;
		}
	}

	if (myPowerUpType == ePowerUpType::FIRERATEBOOST || myPowerUpType == ePowerUpType::HOMING)
	{
		myPowerUpDuration -= aDeltaTime;
		if (myPowerUpDuration <= 0.f)
		{
			myPowerUpType = ePowerUpType::NO_POWERUP;
			myPowerUpCoolDownReducer = 1.f;
			myPowerUpDuration = 0.f;
			myPowerUpValue = 0.f;
			myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || myPowerUpType == ePowerUpType::HOMING, eGUINoteType::HOMING_TARGET));
		}
	}
}

void ShootingComponent::ReceiveNote(const ShootNote& aShootNote)
{
	if (myPowerUpType != ePowerUpType::EMP)
	{
		if (myWeapons[myCurrentWeaponID].myCurrentTime == myWeapons[myCurrentWeaponID].myCoolDownTime)
		{
			for (int i = 0; i < myWeapons[myCurrentWeaponID].myBulletsPerShot; ++i)
			{
				CU::Matrix44<float> orientation = myEntity.myOrientation;
				orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f)
					+ (myWeapons[myCurrentWeaponID].myPosition * myEntity.myOrientation));
				CU::Vector3<float> dir = aShootNote.myDirection;
				if (myWeapons[myCurrentWeaponID].mySpread > 0)
				{
					float max = float(myWeapons[myCurrentWeaponID].mySpread);
					float min = float(-myWeapons[myCurrentWeaponID].mySpread);

					float randomSpreadX = CU::Math::RandomRange<float>(min, max) / 100.f;
					float randomSpreadY = CU::Math::RandomRange<float>(min, max) / 100.f;

					CU::Matrix44<float> rotation;
					rotation.myMatrix[8] = randomSpreadX;
					rotation.myMatrix[9] = randomSpreadY;

					CU::Vector4<float> pos = orientation.GetPos();
					orientation = rotation * orientation;
					orientation.SetPos(pos);

					dir = dir * rotation;
				}

				CU::Matrix44<float> rotation;
				rotation.myMatrix[8] = aShootNote.myEnititySteering.x;
				rotation.myMatrix[9] = -aShootNote.myEnititySteering.y;

				CU::Vector4<float> pos = orientation.GetPos();
				orientation = rotation * orientation;
				orientation.SetPos(pos);

				PostMaster::GetInstance()->SendMessage(BulletMessage(myWeapons[myCurrentWeaponID].myBulletType
					, orientation, myEntity.GetType(), aShootNote.myEnitityVelocity
					, dir
					, myPowerUpType == ePowerUpType::HOMING || myWeapons[myCurrentWeaponID].myIsHoming ? myHomingTarget : nullptr ));
				myWeapons[myCurrentWeaponID].myCurrentTime = 0.f;
			}
		}
	}
	else
	{
		myPowerUpType = ePowerUpType::NO_POWERUP;
		PostMaster::GetInstance()->SendMessage(PowerUpMessage(ePowerUpType::EMP, myEntity.myOrientation.GetPos(), myPowerUpValue, myPowerUpDuration));
		myPowerUpValue = 0.f;
		myPowerUpDuration = 0.f;
	}
}

void ShootingComponent::ReceiveNote(const InputNote& aMessage)
{
	SetCurrentWeaponID(aMessage.myKey);
}

void ShootingComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::EMP 
		|| aNote.myType == ePowerUpType::HOMING)
	{
		myPowerUpCoolDownReducer = 1.f;
		myPowerUpDuration = aNote.myDuration;
		myPowerUpType = aNote.myType;
		myPowerUpValue = aNote.myValue;
		myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || myPowerUpType == ePowerUpType::HOMING, eGUINoteType::HOMING_TARGET));
	}
	else if (aNote.myType == ePowerUpType::FIRERATEBOOST)
	{
		myPowerUpCoolDownReducer = aNote.myValue;
		myPowerUpDuration = aNote.myDuration;
		myPowerUpType = aNote.myType;
		myPowerUpValue = 0.f;
	}
}

void ShootingComponent::AddWeapon(const WeaponDataType& aWeapon)
{
	WeaponData newWeapon;

	newWeapon.myBulletsPerShot = aWeapon.myBulletsPerShot;
	newWeapon.myCoolDownTime = aWeapon.myCoolDownTime;
	newWeapon.myCurrentTime = aWeapon.myCoolDownTime;
	newWeapon.myPosition = aWeapon.myPosition;
	newWeapon.myIsHoming = aWeapon.myIsHoming;
	newWeapon.mySpread = aWeapon.mySpread;
	newWeapon.myType = aWeapon.myType;
	newWeapon.myMultiplier = 1;

	newWeapon.myBulletType = ConvertToBulletEnum(aWeapon.myBulletType);

	if (newWeapon.myBulletType == eBulletType::COUNT)
	{
		std::string errorMessage = "[ShootingComponent] No bullet with name " + aWeapon.myBulletType;
		DL_ASSERT(errorMessage.c_str());
	}

	newWeapon.myID = myWeapons.Size();
	myCurrentWeaponID = newWeapon.myID;
	myWeapons.Add(newWeapon);
	myHasWeapon = true;
}

void ShootingComponent::UpgradeWeapon(const WeaponDataType& aWeapon, int aWeaponID)
{
	if (aWeaponID > myWeapons.Size())
	{
		std::string errorMessage = "[ShootingComponent] Tried to upgrade non existing weapon with ID " + aWeaponID;
		DL_ASSERT(errorMessage.c_str());
	}

	myWeapons[aWeaponID].myBulletsPerShot = aWeapon.myBulletsPerShot;
	myWeapons[aWeaponID].myCoolDownTime = aWeapon.myCoolDownTime;
	myWeapons[aWeaponID].myCurrentTime = aWeapon.myCoolDownTime;
	myWeapons[aWeaponID].myPosition = aWeapon.myPosition;
	myWeapons[aWeaponID].myIsHoming = aWeapon.myIsHoming;
	myWeapons[aWeaponID].mySpread = aWeapon.mySpread;
	myWeapons[aWeaponID].myType = aWeapon.myType;
	myWeapons[aWeaponID].myMultiplier = 1;
	myWeapons[aWeaponID].myBulletType = ConvertToBulletEnum(aWeapon.myBulletType);

	if (myWeapons[aWeaponID].myBulletType == eBulletType::COUNT)
	{
		std::string errorMessage = "[ShootingComponent] No bullet with name " + aWeapon.myBulletType;
		DL_ASSERT(errorMessage.c_str());
	}

	myWeapons[aWeaponID].myID = aWeaponID;
	myCurrentWeaponID = aWeaponID;
}

void ShootingComponent::SetCurrentWeaponID(int anID)
{
	myCurrentWeaponID = anID;

	if (anID >= myWeapons.Size())
	{
		myCurrentWeaponID = myWeapons.Size() - 1;
	}
	myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || myPowerUpType == ePowerUpType::HOMING, eGUINoteType::HOMING_TARGET));
}