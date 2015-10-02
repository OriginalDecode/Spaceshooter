#include "stdafx.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

void WeaponFactory::LoadProjectiles(const std::string& aRootFilePath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aRootFilePath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");

	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string projectilePath = "";
		rootDocument.ForceReadAttribute(e, "src", projectilePath);
		if (projectilePath != "")
		{
			LoadProjectile(projectilePath);
		}
	}

	rootDocument.CloseDocument();
}

void WeaponFactory::LoadWeapons(const std::string& aRootFilePath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aRootFilePath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");

	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string weaponPath = "";
		rootDocument.ForceReadAttribute(e, "src", weaponPath);
		if (weaponPath != "")
		{
			LoadWeapon(weaponPath);
		}
	}

	rootDocument.CloseDocument();
}

ProjectileDataType WeaponFactory::GetProjectile(const std::string& aProjectileType)
{
	auto it = myProjectileTypes.find(aProjectileType);
	return it->second;
}

WeaponDataType WeaponFactory::GetWeapon(const std::string& aWeaponName)
{
	auto it = myWeaponsTypes.find(aWeaponName);
	return it->second;
}

void WeaponFactory::LoadProjectile(const std::string& aProjectileFilePath)
{
	XMLReader projectileDocument;
	projectileDocument.OpenDocument(aProjectileFilePath);

	tinyxml2::XMLElement* rootElement = projectileDocument.FindFirstChild("Projectile");

	ProjectileDataType projectileType;

	projectileDocument.ForceReadAttribute(rootElement, "name", projectileType.myType);

	for (tinyxml2::XMLElement* e = projectileDocument.FindFirstChild(rootElement); e != nullptr;
		e = projectileDocument.FindNextElement(e))
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("model").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "path", projectileType.myModelPath);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("effect").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "path", projectileType.myEffectPath);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("maxAmount").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "value", projectileType.myMaxBullet);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("lifeTime").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "value", projectileType.myLifeTime);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("speed").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "value", projectileType.mySpeed);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("damage").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "value", projectileType.myDamage);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("sphere").c_str()) == 0)
		{
			projectileDocument.ForceReadAttribute(e, "radius", projectileType.myCollisionSphereRadius);
		}
	}

	myProjectileTypes.insert(std::pair<std::string, ProjectileDataType>(projectileType.myType, projectileType));

	projectileDocument.CloseDocument();
}

void WeaponFactory::LoadWeapon(const std::string& aWeaponFilePath)
{
	XMLReader weaponDocument;
	weaponDocument.OpenDocument(aWeaponFilePath);

	tinyxml2::XMLElement* rootElement = weaponDocument.FindFirstChild("Weapon");

	WeaponDataType weaponDataType;

	weaponDocument.ForceReadAttribute(rootElement, "name", weaponDataType.myType);

	for (tinyxml2::XMLElement* e = weaponDocument.FindFirstChild(rootElement); e != nullptr;
		e = weaponDocument.FindNextElement(e))
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("cooldown").c_str()) == 0)
		{
			weaponDocument.ForceReadAttribute(e, "value", weaponDataType.myCoolDownTime);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("spread").c_str()) == 0)
		{
			weaponDocument.ForceReadAttribute(e, "value", weaponDataType.mySpread);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("position").c_str()) == 0)
		{
			weaponDocument.ForceReadAttribute(e, "x", weaponDataType.myPosition.myX);
			weaponDocument.ForceReadAttribute(e, "y", weaponDataType.myPosition.myY);
			weaponDocument.ForceReadAttribute(e, "z", weaponDataType.myPosition.myZ);
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("bullet").c_str()) == 0)
		{
			weaponDocument.ForceReadAttribute(e, "type", weaponDataType.myBulletType);
		}
	}

	myWeaponsTypes.insert(std::pair<std::string, WeaponDataType>(weaponDataType.myType, weaponDataType));

	weaponDocument.CloseDocument();
}