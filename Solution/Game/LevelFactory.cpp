#include "stdafx.h"
#include "AIComponent.h"
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include <CommonHelper.h>
#include "ConversationManager.h"
#include "DefendMessage.h"
#include "DirectionalLight.h"
#include "HealthComponent.h"
#include "EffectContainer.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include "EmitterManager.h"
#include "EventManager.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "InputComponent.h"
#include "Instance.h"
#include "LevelFactory.h"
#include "Level.h"
#include "ModelLoader.h"
#include "MissionManager.h"
#include "ParticleEmitterComponent.h"
#include "PointLight.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PropComponent.h"
#include "PhysicsComponent.h"
#include <Scene.h>
#include "ShieldComponent.h"
#include "ShootingComponent.h"
#include "SoundComponent.h"
#include <SpotLight.h>
#include <Sprite.h>
#include "StreakEmitterComponent.h"
#include "WeaponFactory.h"
#include <XMLReader.h>
#include <thread>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper, Entity& aPlayer)
	: myInputWrapper(anInputWrapper)
	, myPlayer(&aPlayer)
	, myCurrentLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
	, myDirectionalLights(4)
	, myPointLights(4)
	, mySpotLights(4)
	, myIsLoading(false)
	, myLoadLevelThread(nullptr)
{
	LoadLevelListFromXML(aLevelListPath);
}

LevelFactory::~LevelFactory()
{
	delete myLoadLevelThread;
	myLoadLevelThread = nullptr;
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
	myIsLoading = true;
	delete myCurrentLevel;
	myCurrentLevel = nullptr;

	myCurrentLevel = new Level(myInputWrapper);

	delete myLoadLevelThread;
	myLoadLevelThread = nullptr;

	myLoadLevelThread = new std::thread(&LevelFactory::ReadXML, this, myLevelPaths[myCurrentID]);
	//myLoadLevelThread = new std::thread();

	//ReadXML(myLevelPaths[myCurrentID]);

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
		if (myCurrentID >= 10)
		{
			DL_ASSERT("[LevelFactory] Can't handle level ID with two digits.");
		}
	}
	reader.CloseDocument();
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
	myCurrentLevel->myEmitterManager = new EmitterManager();

	myDirectionalLights.DeleteAll();
	myPointLights.DeleteAll();
	mySpotLights.DeleteAll();

	if (myPlayer == nullptr)
	{
		LoadPlayer();
	}
	else
	{
		myCurrentLevel->myPlayer = myPlayer;
	}

	ReadLevelSettings();
	myCurrentLevel->myEntities.Add(myCurrentLevel->myPlayer);
	myCurrentLevel->myCamera = new Prism::Camera(myCurrentLevel->myPlayer->myOrientation);
	myCurrentLevel->myPlayer->GetComponent<GUIComponent>()->SetCamera(myCurrentLevel->myCamera);

	myCurrentLevel->myCollisionManager->Add(myCurrentLevel->myPlayer->GetComponent<CollisionComponent>(), eEntityType::PLAYER);

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

	CU::Vector3<float> playerPos;
	CU::Vector3<float> playerRot;
	tinyxml2::XMLElement* playerPosElement;
	playerPosElement = reader.ForceFindFirstChild(levelElement, "PlayerStartTranslate");
	playerPosElement = reader.ForceFindFirstChild(playerPosElement, "position");
	reader.ForceReadAttribute(playerPosElement, "X", playerPos.x);
	reader.ForceReadAttribute(playerPosElement, "Y", playerPos.y);
	reader.ForceReadAttribute(playerPosElement, "Z", playerPos.z);
	
	playerPosElement = reader.ForceFindFirstChild(levelElement, "PlayerStartTranslate");
	playerPosElement = reader.ForceFindFirstChild(playerPosElement, "rotation");
	reader.ForceReadAttribute(playerPosElement, "X", playerRot.x);
	reader.ForceReadAttribute(playerPosElement, "Y", playerRot.y);
	reader.ForceReadAttribute(playerPosElement, "Z", playerRot.z);

	myPlayer->myOrientation = CU::Matrix44f();

	myPlayer->myOrientation = myPlayer->myOrientation.CreateRotateAroundX(playerRot.x) * myPlayer->myOrientation;
	myPlayer->myOrientation = myPlayer->myOrientation.CreateRotateAroundY(playerRot.y) * myPlayer->myOrientation;
	myPlayer->myOrientation = myPlayer->myOrientation.CreateRotateAroundZ(playerRot.z) * myPlayer->myOrientation;

	myPlayer->myOrientation = CU::GetOrientation(myPlayer->myOrientation, playerRot);

	myPlayer->myOrientation.SetPos(playerPos * 10.f);
	myPlayer->myOriginalOrientation = myPlayer->myOrientation;

	myPlayer->Reset();

	myCurrentLevel->myConversationManager = new ConversationManager(conversationXML);
	myCurrentLevel->myMissionManager = new MissionManager(*myCurrentLevel, *myCurrentLevel->myPlayer, missionXML);
	myCurrentLevel->myEventManager = new EventManager(eventXML, *myCurrentLevel->myConversationManager);

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "skysphere"), "source", skySphere);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "cubemap"), "source", cubeMap);

	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap(cubeMap);

	SetSkySphere(skySphere, "Data/Resource/Shader/S_effect_skybox.fx");

	LoadLights(reader, levelElement);
	LoadProps(reader, levelElement);
	LoadDefendables(reader, levelElement);
	LoadStructures(reader, levelElement);
	LoadTriggers(reader, levelElement);
	LoadPowerups(reader, levelElement);
	
	reader.CloseDocument();

	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		if (myCurrentLevel->myEntities[i]->GetComponent<AIComponent>() != nullptr)
		{
			std::string targetName = myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->GetTargetName();
			Entity* target = myCurrentLevel->GetEntityWithName(targetName);
			myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(myCurrentLevel->myPlayer, myCurrentLevel->myPlayer);
			if (target != nullptr)
			{
				myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(target, myCurrentLevel->myPlayer);
			}
		}

		if (myCurrentLevel->myEntities[i]->GetComponent<ParticleEmitterComponent>() != nullptr)
		{
			myCurrentLevel->myEmitterManager->AddEmitter(myCurrentLevel->myEntities[i]->GetComponent<ParticleEmitterComponent>());
		}
	}

	for (int i = 0; i < static_cast<int>(eBulletType::COUNT); ++i)
	{
		if (i <= 8)
		{
			for (int j = 0; j < myCurrentLevel->myBulletManager->GetBullet(i)->myPlayerBullets.Size(); ++j)
			{
				if (myCurrentLevel->myBulletManager->GetBullet(i)->myPlayerBullets[j]->GetComponent<ParticleEmitterComponent>() != nullptr)
				{
					myCurrentLevel->myEmitterManager->AddEmitter(myCurrentLevel->myBulletManager->GetBullet(i)->myPlayerBullets[j]->GetComponent<ParticleEmitterComponent>());
				}
				if (myCurrentLevel->myBulletManager->GetBullet(i)->myEnemyBullets[j]->GetComponent<ParticleEmitterComponent>() != nullptr)
				{
					myCurrentLevel->myEmitterManager->AddEmitter(myCurrentLevel->myBulletManager->GetBullet(i)->myEnemyBullets[j]->GetComponent<ParticleEmitterComponent>());
				}
			}
		}
	}
	AddToScene();

	myCurrentLevel->myMissionManager->Init();

	myCurrentLevel->myEMP = new Entity(eEntityType::EMP, *myCurrentLevel->myScene, Prism::eOctreeType::NOT_IN_OCTREE);
	myCurrentLevel->myEMP->AddComponent<GraphicsComponent>()->Init("Data/Resource/Model/Weapon/SM_emp_boxsphere.fbx"
		, "Data/Resource/Shader/S_effect_emp.fx");


	while (Prism::Engine::GetInstance()->GetModelLoader()->IsLoading() == true)
	{

	}

	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::LEVEL_IS_LOADED));


	myIsLoading = false;
}

void LevelFactory::ReadLevelSettings()
{
	XMLReader reader;
	std::string settingsPath = "Data/Level/Level0" + std::to_string(myCurrentID - 1) + "/L_level_0" + std::to_string(myCurrentID - 1) + "_settings.xml";
	reader.OpenDocument(settingsPath);

	std::string firstWeapon = "";
	std::string secondWeapon = "";
	std::string thirdWeapon = "";
	reader.ReadAttribute(reader.FindFirstChild("startWeapon"), "first", firstWeapon);
	reader.ReadAttribute(reader.FindFirstChild("startWeapon"), "second", secondWeapon);
	reader.ReadAttribute(reader.FindFirstChild("startWeapon"), "third", thirdWeapon);

	if (firstWeapon != "")
	{
		myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->UpgradeWeapon(myCurrentLevel->myWeaponFactory->GetWeapon(firstWeapon), 0); // replace these with UpgradeWeapon later
	}
	if (secondWeapon != "")
	{
		myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->UpgradeWeapon(myCurrentLevel->myWeaponFactory->GetWeapon(secondWeapon), 1);
	}
	if (thirdWeapon != "")
	{
		myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->UpgradeWeapon(myCurrentLevel->myWeaponFactory->GetWeapon(thirdWeapon), 2);
	}

	tinyxml2::XMLElement* skipableElement = reader.FindFirstChild("skipable");
	if (skipableElement != nullptr)
	{
		myCurrentLevel->myIsSkipable = true;
	}

	reader.CloseDocument();
}

void LevelFactory::LoadLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	LoadDirectionalLights(aReader, aLevelElement);



}

void LevelFactory::LoadDirectionalLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "directionallight"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "directionallight"))
	{
		tinyxml2::XMLElement* directionalElement = aReader.ForceFindFirstChild(entityElement, "rotation");

		Prism::DirectionalLight* newDirLight = new Prism::DirectionalLight();

		

		CU::Vector3<float> lightDirection;
		aReader.ForceReadAttribute(directionalElement, "X", lightDirection.x);
		aReader.ForceReadAttribute(directionalElement, "Y", lightDirection.y);
		aReader.ForceReadAttribute(directionalElement, "Z", lightDirection.z);

		CU::Matrix44<float> orientation;
		
		CU::GetOrientation(orientation, lightDirection);
		CU::Vector3<float> direction(0.f, 0.f, 1.f);

		direction = direction * orientation;

		newDirLight->SetDir(direction);

		//newDirLight->SetOrientation(orientation);
		//newDirLight->SetDir(lightDirection);

		directionalElement = aReader.ForceFindFirstChild(entityElement, "color");

		CU::Vector4<float> lightColor;
		aReader.ForceReadAttribute(directionalElement, "R", lightColor.myR);
		aReader.ForceReadAttribute(directionalElement, "G", lightColor.myG);
		aReader.ForceReadAttribute(directionalElement, "B", lightColor.myB);
		aReader.ForceReadAttribute(directionalElement, "A", lightColor.myA);
		newDirLight->SetColor(lightColor);

		myDirectionalLights.Add(newDirLight);
	}
}

void LevelFactory::LoadProps(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "prop"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity(eEntityType::PROP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		aReader.ForceReadAttribute(entityElement, "propType", propType);
		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, propType);

		newEntity->AddComponent<PropComponent>()->Init("", "");
		FillDataPropOrDefendable(aReader, entityElement, newEntity);
	}
}

void LevelFactory::LoadDefendables(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "defendable"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "defendable"))
	{
		Entity* newEntity = new Entity(eEntityType::DEFENDABLE, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		aReader.ForceReadAttribute(entityElement, "propType", propType);
		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, propType);

		std::string defendName;
		aReader.ForceReadAttribute(entityElement, "defendName", defendName);
		defendName = CU::ToLower(defendName);

		newEntity->AddComponent<PropComponent>()->Init(defendName, "");
		FillDataPropOrDefendable(aReader, entityElement, newEntity);
	}
}

void LevelFactory::LoadStructures(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "structure"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "structure"))
	{
		Entity* newEntity = new Entity(eEntityType::STRUCTURE, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		aReader.ForceReadAttribute(entityElement, "propType", propType);
		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, propType);

		std::string structureName;
		aReader.ForceReadAttribute(entityElement, "structureName", structureName);
		structureName = CU::ToLower(structureName);

		newEntity->AddComponent<PropComponent>()->Init("", structureName);
		FillDataPropOrDefendable(aReader, entityElement, newEntity);
	}
}

void LevelFactory::LoadTriggers(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "trigger"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "trigger"))
	{
		myCurrentLevel->AddTrigger(aReader, entityElement);
	}
}

void LevelFactory::LoadPowerups(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "powerup"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);

		tinyxml2::XMLElement* powerUpElement = aReader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> powerUpPosition;
		aReader.ForceReadAttribute(powerUpElement, "X", powerUpPosition.x);
		aReader.ForceReadAttribute(powerUpElement, "Y", powerUpPosition.y);
		aReader.ForceReadAttribute(powerUpElement, "Z", powerUpPosition.z);
		newEntity->myOriginalOrientation.SetPos(powerUpPosition*10.f);
		newEntity->myOrientation = newEntity->myOriginalOrientation;

		powerUpElement = aReader.ForceFindFirstChild(entityElement, "type");
		std::string powerUp;
		aReader.ForceReadAttribute(powerUpElement, "powerup", powerUp);

		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myCurrentLevel->myPlayer);
		myCurrentLevel->myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);

		myCurrentLevel->myEntities.Add(newEntity);
	}
}

void LevelFactory::LoadPlayer()
{

	myCurrentLevel->myPlayer = new Entity(eEntityType::PLAYER, *myCurrentLevel->myScene, Prism::eOctreeType::DYNAMIC);
	myCurrentLevel->myPlayer->AddComponent<GraphicsComponent>()->Init("Data/Resource/Model/Player/SM_Cockpit.fbx"
		, "Data/Resource/Shader/S_effect_pbl.fx");
	myCurrentLevel->myPlayer->AddComponent<InputComponent>()->Init(*myCurrentLevel->myInputWrapper);
	myCurrentLevel->myPlayer->AddComponent<ShootingComponent>();
	myCurrentLevel->myPlayer->AddComponent<CollisionComponent>()->Init(7.5f);
	myCurrentLevel->myPlayer->AddComponent<ShieldComponent>()->Init();
	myCurrentLevel->myPlayer->AddComponent<PhysicsComponent>()->Init(1, { 0, 0, 0 });
	myCurrentLevel->myPlayer->AddComponent<SoundComponent>();

	//myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("W_gun_machine_level_1")); // replace these with UpgradeWeapon later
	//myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("W_gun_shotgun_level_1"));
	//myCurrentLevel->myPlayer->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("W_gun_rocket_launcher_level_1"));

	XMLReader reader;
	reader.OpenDocument("Data/Setting/SET_player.xml");
	int health = 0;
	bool invulnerable = false;
	reader.ReadAttribute(reader.FindFirstChild("life"), "value", health);
	reader.ReadAttribute(reader.FindFirstChild("life"), "invulnerable", invulnerable);

	myCurrentLevel->myPlayer->AddComponent<HealthComponent>()->Init(health, invulnerable);
	myCurrentLevel->myCollisionManager->Add(myCurrentLevel->myPlayer->GetComponent<CollisionComponent>(), eEntityType::PLAYER);

	//myCurrentLevel->myCamera = new Prism::Camera(myCurrentLevel->myPlayer->myOrientation);
	myCurrentLevel->myPlayer->AddComponent<GUIComponent>();
	float maxMetersToEnemies = 0;
	reader.ReadAttribute(reader.ForceFindFirstChild("maxdistancetoenemiesinGUI"), "meters", maxMetersToEnemies);
	myCurrentLevel->myPlayer->GetComponent<GUIComponent>()->Init(maxMetersToEnemies);
	myPlayer = myCurrentLevel->myPlayer;
	myPlayer->SetName("player");
	reader.CloseDocument();	
}

void LevelFactory::FillDataPropOrDefendable(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement, Entity* aEntityToCreate)
{
	tinyxml2::XMLElement* propElement = aReader.ForceFindFirstChild(aLevelElement, "position");
	CU::Vector3<float> propPosition;
	aReader.ForceReadAttribute(propElement, "X", propPosition.x);
	aReader.ForceReadAttribute(propElement, "Y", propPosition.y);
	aReader.ForceReadAttribute(propElement, "Z", propPosition.z);
	aEntityToCreate->myOriginalOrientation.SetPos(propPosition*10.f);

	propElement = aReader.ForceFindFirstChild(aLevelElement, "rotation");
	CU::Vector3<float> propRotation;
	aReader.ForceReadAttribute(propElement, "X", propRotation.x);
	aReader.ForceReadAttribute(propElement, "Y", propRotation.y);
	aReader.ForceReadAttribute(propElement, "Z", propRotation.z);

	aEntityToCreate->myOriginalOrientation = CU::GetOrientation(aEntityToCreate->myOriginalOrientation, propRotation);

	//if (aEntityToCreate->GetType() == eEntityType::PROP)
	//{
	//	int health = 30;
	//	aEntityToCreate->AddComponent<HealthComponent>()->Init(health);
	//}

#ifdef _DEBUG //REMOVE THIS STUFF WHEN HENRIK HAS UPDATED Entities
	if (aEntityToCreate->GetComponent<HealthComponent>() == nullptr)
	{
		int health = 30;
		aEntityToCreate->AddComponent<HealthComponent>()->Init(health);
	}
#endif

	DL_ASSERT_EXP(aEntityToCreate->GetComponent<HealthComponent>() != nullptr, aEntityToCreate->GetName() + ": Prop without healthcomponent has been loaded.");

	aEntityToCreate->myOrientation = aEntityToCreate->myOriginalOrientation;
	myCurrentLevel->myEntities.Add(aEntityToCreate);
	myCurrentLevel->myCollisionManager->Add(myCurrentLevel->myEntities.GetLast()->GetComponent<CollisionComponent>(), aEntityToCreate->GetType());
}

void LevelFactory::AddToScene()
{
	myCurrentLevel->myScene->SetCamera(myCurrentLevel->myCamera);

	for (int i = 0; i < myDirectionalLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myDirectionalLights[i]);
	}

	for (int i = 0; i < myPointLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myPointLights[i]);
	}

	for (int i = 0; i < mySpotLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(mySpotLights[i]);
	}

	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myCurrentLevel->myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myCurrentLevel->myScene->AddInstance(gfxComp->GetInstance());
		}
	}
}

void LevelFactory::SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName)
{
	Prism::ModelProxy* skySphere = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		aModelFilePath, aEffectFileName);
	delete myCurrentLevel->mySkySphere;
	myCurrentLevel->mySkySphereCullingRadius = 10.f;
	myCurrentLevel->mySkySphere = new Prism::Instance(*skySphere
		, myPlayer->GetComponent<InputComponent>()->GetSkyOrientation(), Prism::eOctreeType::NOT_IN_OCTREE
		, myCurrentLevel->mySkySphereCullingRadius);
}

void LevelFactory::Cleanup()
{
	myLoadLevelThread->join();
	delete myLoadLevelThread;
	myLoadLevelThread = nullptr;
}