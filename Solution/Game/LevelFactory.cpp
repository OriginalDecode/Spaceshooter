#include "stdafx.h"
#include "AIComponent.h"
#include "BulletManager.h"
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "ConversationManager.h"
#include "DefendMessage.h"
#include "DirectionalLight.h"
#include "HealthComponent.h"
#include "EffectContainer.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include "EventManager.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "LevelFactory.h"
#include "Level.h"
#include "MissionManager.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PropComponent.h"
#include <Scene.h>
#include "WeaponFactory.h"
#include <XMLReader.h>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper)
	: myInputWrapper(anInputWrapper)
	, myCurrentLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
{
	LoadLevelListFromXML(aLevelListPath);
}

LevelFactory::~LevelFactory()
{
}

Level* LevelFactory::LoadLevel(const int& anID)
{
	if (myLevelPaths.find(anID) == myLevelPaths.end())
	{
		DL_ASSERT("[LevelFactory] Non-existing ID in LoadLevel! ID must correspond with levelList.xml");
	}
	myCurrentID = anID;

	return LoadCurrentLevel();
}

Level* LevelFactory::LoadCurrentLevel()
{
	delete myCurrentLevel;
	myCurrentLevel = nullptr;

	myCurrentLevel = new Level(myInputWrapper);
	ReadXML(myLevelPaths[myCurrentID]);

	return myCurrentLevel;
}

Level* LevelFactory::LoadNextLevel()
{
	if (IsLastLevel() == true)
	{
		return myCurrentLevel;
	}

	return LoadLevel(myCurrentID + 1);
}

void LevelFactory::ReadXML(const std::string& aFilePath)
{
	myCurrentLevel->myScene = new Prism::Scene();
	myCurrentLevel->myWeaponFactory = new WeaponFactory();
	myCurrentLevel->myWeaponFactory->LoadWeapons("Data/Script/LI_list_weapon.xml");
	myCurrentLevel->myWeaponFactory->LoadProjectiles("Data/Script/LI_list_projectile.xml");
	myCurrentLevel->myEntityFactory = new EntityFactory(myCurrentLevel->myWeaponFactory);
	myCurrentLevel->myEntityFactory->LoadEntites("Data/Script/LI_list_entity.xml");
	myCurrentLevel->myCollisionManager = new CollisionManager();
	myCurrentLevel->myBulletManager = new BulletManager(*myCurrentLevel->myCollisionManager, *myCurrentLevel->myScene);
	myCurrentLevel->myBulletManager->LoadFromFactory(myCurrentLevel->myWeaponFactory, myCurrentLevel->myEntityFactory, "Data/Script/LI_list_projectile.xml");

	Prism::DirectionalLight* dirLight = new Prism::DirectionalLight();
	dirLight->SetColor({ 0.5f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, 0.5f, -1.f });
	myCurrentLevel->myDirectionalLights.Add(dirLight);

	myCurrentLevel->LoadPlayer();

	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFilePath);
	tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("root");
	levelElement = reader.ForceFindFirstChild(levelElement, "scene");
	std::string skySphere;
	std::string cubeMap;
	std::string missionXML;
	std::string eventXML;
	std::string conversationXML;

	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "missionxml"), "source", missionXML);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "eventxml"), "source", eventXML);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "conversationxml"), "source", conversationXML);

	myCurrentLevel->myConversationManager = new ConversationManager(conversationXML);
	myCurrentLevel->myMissionManager = new MissionManager(*myCurrentLevel, *myCurrentLevel->myPlayer, missionXML);
	myCurrentLevel->myEventManager = new EventManager(eventXML, *myCurrentLevel->myConversationManager);

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "skysphere"), "source", skySphere);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "cubemap"), "source", cubeMap);

	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap(cubeMap);

	myCurrentLevel->SetSkySphere(skySphere, "Data/Resource/Shader/S_effect_skybox.fx");

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "directionallight"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "directionallight"))
	{
		tinyxml2::XMLElement* directionalElement = reader.ForceFindFirstChild(entityElement, "rotation");

		Prism::DirectionalLight* newDirLight = new Prism::DirectionalLight();

		CU::Vector3<float> lightDirection;
		reader.ForceReadAttribute(directionalElement, "X", lightDirection.x);
		reader.ForceReadAttribute(directionalElement, "Y", lightDirection.y);
		reader.ForceReadAttribute(directionalElement, "Z", lightDirection.z);
		newDirLight->SetDir(lightDirection);

		directionalElement = reader.ForceFindFirstChild(entityElement, "color");

		CU::Vector4<float> lightColor;
		reader.ForceReadAttribute(directionalElement, "R", lightColor.myR);
		reader.ForceReadAttribute(directionalElement, "G", lightColor.myG);
		reader.ForceReadAttribute(directionalElement, "B", lightColor.myB);
		reader.ForceReadAttribute(directionalElement, "A", lightColor.myA);
		newDirLight->SetColor(lightColor);

		//myScene->AddLight(newDirLight);
		myCurrentLevel->myDirectionalLights.Add(newDirLight);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "prop"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity(eEntityType::PROP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		reader.ForceReadAttribute(entityElement, "propType", propType);
		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, propType);

		std::string defendName;
		reader.ReadAttribute(entityElement, "defendName", defendName);
		defendName = CU::ToLower(defendName);

		tinyxml2::XMLElement* propElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> propPosition;
		reader.ForceReadAttribute(propElement, "X", propPosition.x);
		reader.ForceReadAttribute(propElement, "Y", propPosition.y);
		reader.ForceReadAttribute(propElement, "Z", propPosition.z);
		newEntity->myOrientation.SetPos(propPosition*10.f);

		propElement = reader.ForceFindFirstChild(entityElement, "rotation");
		CU::Vector3<float> propRotation;
		reader.ForceReadAttribute(propElement, "X", propRotation.x);
		reader.ForceReadAttribute(propElement, "Y", propRotation.y);
		reader.ForceReadAttribute(propElement, "Z", propRotation.z);

		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundX(propRotation.x) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundY(propRotation.y) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundZ(propRotation.z) * newEntity->myOrientation;

		newEntity->AddComponent<PropComponent>()->Init(defendName);

		int health = 30;
		newEntity->AddComponent<HealthComponent>()->Init(health);

		myCurrentLevel->myEntities.Add(newEntity);
		myCurrentLevel->myCollisionManager->Add(myCurrentLevel->myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::PROP);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "trigger"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "trigger"))
	{
		myCurrentLevel->AddTrigger(reader, entityElement);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "powerup"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);

		tinyxml2::XMLElement* powerUpElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> powerUpPosition;
		reader.ForceReadAttribute(powerUpElement, "X", powerUpPosition.x);
		reader.ForceReadAttribute(powerUpElement, "Y", powerUpPosition.y);
		reader.ForceReadAttribute(powerUpElement, "Z", powerUpPosition.z);
		newEntity->myOrientation.SetPos(powerUpPosition*10.f);

		powerUpElement = reader.ForceFindFirstChild(entityElement, "type");
		std::string powerUp;
		reader.ForceReadAttribute(powerUpElement, "powerup", powerUp);

		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myCurrentLevel->myPlayer);
		myCurrentLevel->myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);

		myCurrentLevel->myEntities.Add(newEntity);
	}
	reader.CloseDocument();


	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		if (myCurrentLevel->myEntities[i]->GetComponent<AIComponent>() != nullptr)
		{
			std::string targetName = myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->GetTargetName();
			Entity* target = myCurrentLevel->GetEntityWithName(targetName);
			myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(myCurrentLevel->myPlayer);
			if (target != nullptr)
			{
				myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(target);
			}
		}
	}


	myCurrentLevel->myScene->SetCamera(myCurrentLevel->myCamera);

	for (int i = 0; i < myCurrentLevel->myDirectionalLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myCurrentLevel->myDirectionalLights[i]);
	}

	for (int i = 0; i < myCurrentLevel->myPointLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myCurrentLevel->myPointLights[i]);
	}

	for (int i = 0; i < myCurrentLevel->mySpotLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myCurrentLevel->mySpotLights[i]);
	}

	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myCurrentLevel->myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myCurrentLevel->myScene->AddInstance(gfxComp->GetInstance());
		}
	}

	myCurrentLevel->myMissionManager->Init();
}

bool LevelFactory::IsLastLevel() const
{
	if (myLevelPaths.find(myCurrentID + 1) == myLevelPaths.end())
	{
		return true;
	}
	return false;
}

void LevelFactory::LoadLevelListFromXML(const std::string& aXMLPath)
{
	myLevelPaths.clear();
	XMLReader reader;
	reader.OpenDocument(aXMLPath);
	std::string levelPath = "";
	int ID = 0;
	int lastID = ID - 1;

	tinyxml2::XMLElement* levelElement = reader.FindFirstChild("level");
	for (; levelElement != nullptr; levelElement = reader.FindNextElement(levelElement))
	{
		lastID = ID;
		
		reader.ForceReadAttribute(levelElement, "ID", ID);
		reader.ForceReadAttribute(levelElement, "path", levelPath);
		myLevelPaths[ID] = levelPath;

		if (ID - 1 != lastID)
		{
			DL_ASSERT("[LevelFactory] Wrong ID-number in levelList.xml! The numbers should be counting up, in order.");
		}
	}
	reader.CloseDocument();
}