#include "stdafx.h"
#include "WeaponFactory.h"


WeaponFactory::WeaponFactory()
{
}


WeaponFactory::~WeaponFactory()
{
}


void WeaponFactory::LoadWeapons(const std::string& aRootFilePath)
{

}

WeaponData WeaponFactory::GetWeapon(const std::string& aWeaponName)
{
	auto it = myWeaponsTypes.find(aWeaponName);
	return it->second;
}