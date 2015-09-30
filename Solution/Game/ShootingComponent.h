#pragma once
#include "Component.h"
#include "BulletEnums.h"

struct WeaponData
{
	eBulletType myBulletType;
	float myCoolDownTime;
	float myCurrentTime;
	int mySpread;
	int myID;
	CU::Vector3<float> myPosition;
};

class Entity;

class ShootingComponent : public Component
{
public:

	ShootingComponent();
	~ShootingComponent();

	void Update(float aDeltaTime) override;

	void ReceiveMessage(const ShootMessage& aMessage) override;
	void ReceiveMessage(const InputMessage& aMessage) override;

	void ReadFromXML(const std::string aFilePath);

	void AddWeapon(WeaponData aWeapon);

	static int GetID();

	void SetCurrentWeaponID(const unsigned short& anID);

private:

	CU::GrowingArray<WeaponData, unsigned short> myWeapons;

	unsigned short myCurrentWeaponID;
	CU::Vector3<float> mySpawningPointOffset;
};

inline void ShootingComponent::AddWeapon(WeaponData aWeapon)
{
	myWeapons.Add(aWeapon);
}

inline int ShootingComponent::GetID()
{
	return 4;
}

inline void ShootingComponent::SetCurrentWeaponID(const unsigned short& anID)
{
	myCurrentWeaponID = anID;
	
	if (anID > myWeapons.Size())
	{
		myCurrentWeaponID = myWeapons.Size();
	}
}