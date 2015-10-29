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
	, myPowerUps(8)
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
			float totalReducer = 1.f;
			for (int i = 0; i < myPowerUps.Size(); i++)
			{
				totalReducer += myPowerUps[i].myPowerUpCoolDownReducer;
			}
			myWeapons[myCurrentWeaponID].myCurrentTime += aDeltaTime * totalReducer;
		}
	}

	for (int i = myPowerUps.Size() - 1; i >= 0; --i)
	{
		if (myPowerUps[i].myPowerUpType == ePowerUpType::FIRERATEBOOST 
			|| myPowerUps[i].myPowerUpType == ePowerUpType::HOMING)
		{
			myPowerUps[i].myPowerUpDuration -= aDeltaTime;
			if (myPowerUps[i].myPowerUpDuration <= 0.f)
			{
				myPowerUps.RemoveCyclicAtIndex(i);

				myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || HasPowerUp(ePowerUpType::HOMING), eGUINoteType::HOMING_TARGET));
			}
		}
	}
}

void ShootingComponent::ReceiveNote(const ShootNote& aShootNote)
{
	if (HasPowerUp(ePowerUpType::EMP) == false)
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
					, HasPowerUp(ePowerUpType::HOMING) || myWeapons[myCurrentWeaponID].myIsHoming ? myHomingTarget : nullptr));
				myWeapons[myCurrentWeaponID].myCurrentTime = 0.f;
			}
		}
	}
	else
	{
		ActivatePowerUp(ePowerUpType::EMP);
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
		WeaponPowerUp powerUp;
		powerUp.myPowerUpCoolDownReducer = 0.f;
		powerUp.myPowerUpDuration = aNote.myDuration;
		powerUp.myPowerUpType = aNote.myType;
		powerUp.myPowerUpValue = aNote.myValue;
		myPowerUps.Add(powerUp);
		myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || powerUp.myPowerUpType == ePowerUpType::HOMING, eGUINoteType::HOMING_TARGET));
	}
	else if (aNote.myType == ePowerUpType::FIRERATEBOOST)
	{
		WeaponPowerUp powerUp;
		powerUp.myPowerUpCoolDownReducer = aNote.myValue;
		powerUp.myPowerUpDuration = aNote.myDuration;
		powerUp.myPowerUpType = aNote.myType;
		powerUp.myPowerUpValue = 0.f;
		myPowerUps.Add(powerUp);
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
	myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || HasPowerUp(ePowerUpType::HOMING), eGUINoteType::HOMING_TARGET));
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
	myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || HasPowerUp(ePowerUpType::HOMING), eGUINoteType::HOMING_TARGET));
}

void ShootingComponent::SetCurrentWeaponID(int anID)
{
	myCurrentWeaponID = anID;

	if (anID >= myWeapons.Size())
	{
		myCurrentWeaponID = myWeapons.Size() - 1;
	}
	myEntity.SendNote(GUINote(myWeapons[myCurrentWeaponID].myIsHoming || HasPowerUp(ePowerUpType::HOMING), eGUINoteType::HOMING_TARGET));
}

bool ShootingComponent::HasPowerUp(ePowerUpType aPowerUp)
{
	for (int i = 0; i < myPowerUps.Size(); ++i)
	{
		if (myPowerUps[i].myPowerUpType == aPowerUp)
		{
			return true;
		}
	}

	return false;
}

void ShootingComponent::ActivatePowerUp(ePowerUpType aPowerUp)
{
	for (int i = myPowerUps.Size() - 1; i >= 0; --i)
	{
		if (myPowerUps[i].myPowerUpType == aPowerUp)
		{
			PostMaster::GetInstance()->SendMessage(PowerUpMessage(aPowerUp, myEntity.myOrientation.GetPos()
				, myPowerUps[i].myPowerUpValue, myPowerUps[i].myPowerUpDuration));
			myPowerUps.RemoveCyclicAtIndex(i);
			return;
		}
	}
}