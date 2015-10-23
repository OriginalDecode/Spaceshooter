#pragma once
#include "Component.h"
#include "Enums.h"

struct WeaponData
{
	eBulletType myBulletType;
	int myBulletsPerShot;
	int myMultiplier;
	int mySpread;
	int myID;
	float myCoolDownTime;
	float myCurrentTime;
	bool myIsHoming;
	CU::Vector3<float> myPosition;
	std::string myType;
};

class Entity;

struct WeaponDataType;

class ShootingComponent : public Component
{
public:
	ShootingComponent(Entity& aEntity);

	void Update(float aDeltaTime) override;

	void ReceiveNote(const ShootNote& aMessage) override;
	void ReceiveNote(const InputNote& aMessage) override;
	void ReceiveNote(const PowerUpNote& aNote) override;

	void AddWeapon(const WeaponDataType& aWeapon);
	void UpgradeWeapon(const WeaponDataType& aWeapon, int aWeaponID);

	static eComponentType GetType();

	void SetCurrentWeaponID(int anID);

	void SetHomingTarget(Entity* aTarget);

	void ActivateEMP(); // test function

private:

	CU::GrowingArray<WeaponData, int> myWeapons;

	Entity* myHomingTarget;

	int myCurrentWeaponID;

	bool myHasWeapon;
	ePowerUpType myPowerUpType;

	float myPowerUpValue;
	float myPowerUpDuration;
	float myPowerUpCoolDownReducer;
};

inline eComponentType ShootingComponent::GetType()
{
	return eComponentType::SHOOTING;
}

inline void ShootingComponent::SetCurrentWeaponID(int anID)
{
	myCurrentWeaponID = anID;
	
	if (anID >= myWeapons.Size())
	{
		myCurrentWeaponID = myWeapons.Size() - 1;
	}
}

inline void ShootingComponent::ActivateEMP()
{
	myPowerUpValue = 1000.f;
	myPowerUpDuration = 5.f;
}

inline void ShootingComponent::SetHomingTarget(Entity* aTarget)
{
	myHomingTarget = aTarget;
}

inline eBulletType ConvertToBulletEnum(const std::string& aString)
{
	if (aString == "machinegun1")
	{
		return eBulletType::MACHINGUN_BULLET_LEVEL_1;
	}
	else if (aString == "machinegun2")
	{
		return eBulletType::MACHINGUN_BULLET_LEVEL_2;
	}
	else if (aString == "machinegun3")
	{
		return eBulletType::MACHINGUN_BULLET_LEVEL_3;
	}
	else if (aString == "shotgun1")
	{
		return eBulletType::SHOTGUN_BULLET_LEVEL_1;
	}
	else if (aString == "shotgun2")
	{
		return eBulletType::SHOTGUN_BULLET_LEVEL_2;
	}
	else if (aString == "shotgun3")
	{
		return eBulletType::SHOTGUN_BULLET_LEVEL_3;
	}
	else if (aString == "rocket1")
	{
		return eBulletType::ROCKET_MISSILE_LEVEL_1;
	}
	else if (aString == "rocket2")
	{
		return eBulletType::ROCKET_MISSILE_LEVEL_2;
	}
	else if (aString == "rocket3")
	{
		return eBulletType::ROCKET_MISSILE_LEVEL_3;
	}
	return eBulletType::COUNT;
}