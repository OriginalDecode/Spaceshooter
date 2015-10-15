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

private:

	CU::GrowingArray<WeaponData, int> myWeapons;

	int myCurrentWeaponID;
	CU::Vector3<float> mySpawningPointOffset;

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