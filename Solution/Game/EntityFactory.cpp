#include "stdafx.h"

#include "AIComponent.h"
#include "CollisionComponent.h"
#include "BulletComponent.h"
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "HealthComponent.h"
#include <Instance.h>
#include <MathHelper.h>
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include <Scene.h>
#include "ShootingComponent.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

EntityData::EntityData(Prism::Scene& aDummyScene)
	: myEntity(new Entity(eEntityType::NOT_USED, aDummyScene))
{
}

EntityFactory::EntityFactory(WeaponFactory* aWeaponFactory)
	: myWeaponFactoryPointer(aWeaponFactory)
{
	myDummyScene = new Prism::Scene();
}

EntityFactory::~EntityFactory()
{
	for (auto it = myEntities.begin(); it != myEntities.end(); ++it)
	{
		delete it->second.myEntity;
	}

	myEntities.clear();

	delete myDummyScene;
}

void EntityFactory::LoadEntites(const std::string& aEntityRootPath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aEntityRootPath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");

	WATCH_FILE(aEntityRootPath, EntityFactory::ReloadEntity);

	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string entityPath = "";
		rootDocument.ForceReadAttribute(e, "src", entityPath);
		if (entityPath != "")
		{
			LoadEntity(entityPath);
			WATCH_FILE(entityPath, EntityFactory::ReloadEntity);
		}
	}

	rootDocument.CloseDocument();
}

void EntityFactory::LoadEntity(const std::string& aEntityPath)
{
	XMLReader entityDocument;
	entityDocument.OpenDocument(aEntityPath);

	EntityData newEntity(*myDummyScene);

	tinyxml2::XMLElement* rootElement = entityDocument.FindFirstChild("Entity");

	std::string entityName = "";
	entityDocument.ForceReadAttribute(rootElement, "name", entityName);

	if (myEntities.find(entityName) != myEntities.end())
	{
		std::string errorMessage = "[EntityFactory] Entity there is already a object named " + entityName;
		DL_ASSERT(errorMessage.c_str());
	}

	newEntity.myEntity->SetName(entityName);
	ENTITY_LOG("Load entity %s starting", entityName.c_str());
	for (tinyxml2::XMLElement* e = entityDocument.FindFirstChild(rootElement); e != nullptr;
		e = entityDocument.FindNextElement(e))
	{
		std::string childName = e->Name();
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("GraphicsComponent").c_str()) == 0)
		{
			LoadGraphicsComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("AIComponent").c_str()) == 0)
		{
			LoadAIComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("ShootingComponent").c_str()) == 0)
		{
			LoadShootingComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("CollisionComponent").c_str()) == 0)
		{
			LoadCollisionComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("PhysicsComponent").c_str()) == 0)
		{
			LoadPhysicsComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("BulletComponent").c_str()) == 0)
		{
			LoadBulletComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("HealthComponent").c_str()) == 0)
		{
			LoadHealthComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("PowerUpComponent").c_str()) == 0)
		{
			LoadPowerUpComponent(newEntity, entityDocument, e);
			ENTITY_LOG("Entity %s loaded %s", entityName.c_str(), e->Name());
		}
		else
		{
			std::string errorMessage = "[EntityFactory]: Entity could not find the component " 
				+ static_cast<std::string>(e->Name());
			DL_ASSERT(errorMessage.c_str());
		}
	}
	if (entityName != "")
	{
		myEntities.insert(std::pair<std::string, EntityData>(entityName, newEntity));
		ENTITY_LOG("Load entity %s ending", entityName.c_str());
	}
	else
	{
		DL_ASSERT("[EntityFactory]: Entity could not be created missing a name.");
	}

	entityDocument.CloseDocument();
}

void EntityFactory::LoadAIComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aAIComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<AIComponent>();
	for (tinyxml2::XMLElement* e = aAIComponentElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("FollowEntity").c_str()) == 0)
		{
			aDocument.ForceReadAttribute(e, "targetName", aEntityToAddTo.myTargetName);
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Speed").c_str()) == 0)
		{
			aDocument.ForceReadAttribute(e, "min", aEntityToAddTo.myMinSpeed);
			aDocument.ForceReadAttribute(e, "max", aEntityToAddTo.myMaxSpeed);
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("TimeToNextDecision").c_str()) == 0)
		{
			aDocument.ForceReadAttribute(e, "min", aEntityToAddTo.myMinTimeToNextDecision);
			aDocument.ForceReadAttribute(e, "max", aEntityToAddTo.myMaxTimeToNextDecision);
		}
	}
}

void EntityFactory::LoadBulletComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aBulletComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<BulletComponent>();
	for (tinyxml2::XMLElement* e = aBulletComponentElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("lifeTime").c_str()) == 0)
		{
			aDocument.ForceReadAttribute(e, "value", aEntityToAddTo.myMaxTime);
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("damage").c_str()) == 0)
		{
			int damage = 0;

			aDocument.ForceReadAttribute(e, "value", damage);

			aEntityToAddTo.myDamage = static_cast<unsigned short>(damage);
		}
	}
}

void EntityFactory::LoadCollisionComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aCollisionComponenetElement)
{
	aEntityToAddTo.myEntity->AddComponent<CollisionComponent>();

	for (tinyxml2::XMLElement* e = aCollisionComponenetElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("CollisionSphere").c_str()) == 0)
		{
			float radius = 0;

			aDocument.ForceReadAttribute(e, "radius", radius);

			aEntityToAddTo.myCollisionSphereRadius = radius;
		}
	}
}

void EntityFactory::LoadGraphicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aGraphicsComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<GraphicsComponent>();

	for (tinyxml2::XMLElement* e = aGraphicsComponentElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Model").c_str()) == 0)
		{
			std::string modelFile = "";
			std::string effectFile = "";

			aDocument.ForceReadAttribute(e, "modelFile", modelFile);
			aDocument.ForceReadAttribute(e, "effectFile", effectFile);

			aEntityToAddTo.myModelFile = modelFile;
			aEntityToAddTo.myEffectFile = effectFile;

			aEntityToAddTo.myGraphicsType = eEntityDataGraphicsType::MODEL;
		}
		else if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Cube").c_str()) == 0)
		{
			float width = e->FloatAttribute("width");
			float height = e->FloatAttribute("height");
			float depth = e->FloatAttribute("depth");

			aDocument.ForceReadAttribute(e, "width", width);
			aDocument.ForceReadAttribute(e, "height", height);
			aDocument.ForceReadAttribute(e, "depth", depth);

			aEntityToAddTo.myWidth = width;
			aEntityToAddTo.myHeight = height;
			aEntityToAddTo.myDepth = depth;

			aEntityToAddTo.myGraphicsType = eEntityDataGraphicsType::CUBE;
		}
	}
}

void EntityFactory::LoadShootingComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aShootingComponenetElement)
{
	aEntityToAddTo.myEntity->AddComponent<ShootingComponent>();
	aDocument;
	for (tinyxml2::XMLElement* e = aShootingComponenetElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Weapon").c_str()) == 0)
		{
			aDocument.ForceReadAttribute(e, "type", aEntityToAddTo.myWeaponType);
		}
	}
}

void EntityFactory::LoadHealthComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aHealthComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<HealthComponent>();

	for (tinyxml2::XMLElement* e = aHealthComponentElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Health").c_str()) == 0)
		{
			int life = 0;

			aDocument.ForceReadAttribute(e, "value", life);

			aEntityToAddTo.myLife = static_cast<unsigned short>(life);
		}
	}
}

void EntityFactory::LoadPhysicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aPhysicsComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<PhysicsComponent>();
	aDocument;
	aPhysicsComponentElement;
}

void EntityFactory::LoadPowerUpComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aPowerUpComponent)
{
	aEntityToAddTo.myEntity->AddComponent<PowerUpComponent>();

	for (tinyxml2::XMLElement* e = aPowerUpComponent->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("Duration").c_str()) == 0)
		{
			float duration = 0;
			aDocument.ForceReadAttribute(e, "value", duration);
			aEntityToAddTo.myDuration = duration;
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("ShieldStrength").c_str()) == 0)
		{
			int shield = 0;
			aDocument.ForceReadAttribute(e, "value", shield);
			aEntityToAddTo.myShieldStrength = shield;
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("HealthToRecover").c_str()) == 0)
		{
			float health = 0;
			aDocument.ForceReadAttribute(e, "value", health);
			aEntityToAddTo.myHealthToRecover = static_cast<int>(health);
		}
		if (std::strcmp(CU::ToLower(e->Name()).c_str(), CU::ToLower("FireRateMultiplier").c_str()) == 0)
		{
			float firerate = 0;
			aDocument.ForceReadAttribute(e, "value", firerate);
			aEntityToAddTo.myFireRateMultiplier = static_cast<int>(firerate);
		}

	}
}



void EntityFactory::CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag)
{
	auto it = myEntities.find(aEntityTag);
	Entity* sourceEntity = it->second.myEntity;

	aTargetEntity->SetName(sourceEntity->GetName());

	if (sourceEntity->GetComponent<GraphicsComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<GraphicsComponent>();
		switch (it->second.myGraphicsType)
		{
		case eEntityDataGraphicsType::MODEL:
			aTargetEntity->GetComponent<GraphicsComponent>()->Init(it->second.myModelFile.c_str(),
				it->second.myEffectFile.c_str());
			break;
		case eEntityDataGraphicsType::CUBE:
			aTargetEntity->GetComponent<GraphicsComponent>()->InitCube(it->second.myWidth,
				it->second.myHeight, it->second.myDepth);
			break;
		default:
			break;
		}
	}
	if (sourceEntity->GetComponent<AIComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<AIComponent>();
		float speed = CU::Math::RandomRange<float>(it->second.myMinSpeed, it->second.myMaxSpeed);
		float timeToNextDecision = CU::Math::RandomRange<float>(it->second.myMinTimeToNextDecision,
			it->second.myMaxTimeToNextDecision);
		aTargetEntity->GetComponent<AIComponent>()->Init(speed, timeToNextDecision, it->second.myTargetName);
	}
	if (sourceEntity->GetComponent<ShootingComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<ShootingComponent>();
		if (it->second.myWeaponType != "")
		{
			aTargetEntity->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactoryPointer->GetWeapon(it->second.myWeaponType));
		}
	}
	if (sourceEntity->GetComponent<CollisionComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<CollisionComponent>();

		if (it->second.myCollisionSphereRadius > 0)
		{
			aTargetEntity->GetComponent<CollisionComponent>()->Initiate(it->second.myCollisionSphereRadius);
		}
	}
	if (sourceEntity->GetComponent<HealthComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<HealthComponent>();
		aTargetEntity->GetComponent<HealthComponent>()->Init(it->second.myLife);
	}
	if (sourceEntity->GetComponent<PhysicsComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<PhysicsComponent>();
	}
	if (sourceEntity->GetComponent<BulletComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<BulletComponent>();
		if (it->second.myMaxTime > 0 && it->second.myDamage > 0)
		{
			aTargetEntity->GetComponent<BulletComponent>()->Init(it->second.myMaxTime, it->second.myDamage);
		}
	}

	if (sourceEntity->GetComponent<PowerUpComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<PowerUpComponent>();
		
		aTargetEntity->GetComponent<PowerUpComponent>()->Init(aTargetEntity->GetPowerUpType(), it->second.myDuration
			, it->second.myShieldStrength, it->second.myHealthToRecover, it->second.myFireRateMultiplier);

	}
	ENTITY_LOG("Entity %s copying succeded", aTargetEntity->GetName().c_str());
}

void EntityFactory::ReloadEntity(const std::string&)
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::RELOAD_LEVEL));
}