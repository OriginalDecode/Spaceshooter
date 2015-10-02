#pragma once
#include "Enums.h"

struct WeaponData
{
	eBulletType myBulletType;
	float myCoolDownTime;
	float myCurrentTime;
	int mySpread;
	int myID;
	CU::Vector3<float> myPosition;
};

class WeaponFactory
{
public:
	WeaponFactory();
	~WeaponFactory();

	void LoadWeapons(const std::string& aRootFilePath);

	WeaponData GetWeapon(const std::string& aWeaponName);
private:
	std::unordered_map<std::string, WeaponData> myWeaponsTypes;
};

