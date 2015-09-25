#pragma once
#include "Component.h"
#include "BulletEnums.h"

struct WeaponData
{
	eBulletType myBulletType;
	float myCoolDownTime;
	float myCurrentTime;
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

	static int GetID();

private:
	WeaponData* myCurrentWeapon;
	CU::Vector3<float> mySpawningPointOffset;
};

inline int ShootingComponent::GetID()
{
	return 4;
}

