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
};

class Entity;

class ShootingComponent : public Component
{
public:

	ShootingComponent();
	~ShootingComponent();

	void Update(float aDeltaTime) override;

	void ReceiveMessage(const ShootMessage& aMessage) override;
	void Init(CU::Vector3<float> aSpawningPointOffset);

	void ReadFromXML(const std::string aFilePath);

	void AddWeapon(WeaponData aWeapon);

	static int GetID();

private:

	CU::GrowingArray<WeaponData> myWeapons;

	unsigned int myCurrentWeapon;
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

