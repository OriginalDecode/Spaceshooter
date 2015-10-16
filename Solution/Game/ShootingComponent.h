#pragma once
#include "Component.h"
#include "Enums.h"

struct WeaponData
{
	eBulletType myBulletType;
	int mySpread;
	int myID;
	int myBulletsPerShot;
	float myCoolDownTime;
	float myCurrentTime;
	CU::Vector3<float> myPosition;
	std::string myType;
	int myMultiplier;
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

	static eComponentType GetType();

	void SetCurrentWeaponID(int anID);

	void ActivateEMP(); // test function

private:

	CU::GrowingArray<WeaponData, int> myWeapons;

	int myCurrentWeaponID;

	bool myHasWeapon;
	bool myHasEMP;
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
	myHasEMP = true;
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