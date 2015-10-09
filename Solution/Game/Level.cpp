#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "DirectionalLight.h"
#include "EffectContainer.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "Instance.h"
#include <InputWrapper.h>
#include "InputComponent.h"
#include <Intersection.h>
#include "Level.h"
#include "MissionManager.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "PointLight.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PropComponent.h"
#include <Scene.h>
#include "ShootingComponent.h"
#include <sstream>
#include <string>
#include <SpotLight.h>
#include "WeaponFactory.h"
#include <XMLReader.h>

Level::Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper)
	: myEntities(16)
	, myComplete(false)
	, mySkySphere(nullptr)
{
	myScene = new Prism::Scene();
	myWeaponFactory = new WeaponFactory();
	myWeaponFactory->LoadWeapons("Data/weapons/WeaponList.xml");
	myWeaponFactory->LoadProjectiles("Data/weapons/projectiles/ProjectileList.xml");
	myEntityFactory = new EntityFactory(myWeaponFactory);
	myEntityFactory->LoadEntites("Data/entities/EntityList.xml");
	myInputWrapper = aInputWrapper;
	myShowPointLightCube = false;

	myCollisionManager = new CollisionManager();
	myBulletManager = new BulletManager(*myCollisionManager, *myScene);
	myBulletManager->LoadFromFactory(myWeaponFactory, myEntityFactory, "Data/weapons/projectiles/ProjectileList.xml");

	myDirectionalLights.Init(4);
	myPointLights.Init(4);
	mySpotLights.Init(4);

	Prism::DirectionalLight* dirLight = new Prism::DirectionalLight();
	dirLight->SetColor({ 0.5f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, 0.5f, -1.f });
	myDirectionalLights.Add(dirLight);
	
	LoadPlayer();

	WATCH_FILE(aFileName, Level::ReadXML);

	ReadXML(aFileName);

	//Entity* cube = new Entity(eEntityType::PROP, *myScene, "this is a cube");
	//cube->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Primitives/cube.fbx"
	//	, "Data/effect/NoTextureEffect.fx");
	//cube->myOrientation.SetPos({ 300.f, 300.f, 300.f, 1.f });
	//cube->Update(1.f / 30.f);
	//myEntities.Add(cube);

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i]->GetComponent<AIComponent>() != nullptr)
		{
			std::string targetName = myEntities[i]->GetComponent<AIComponent>()->GetTargetName();
			Entity* target = GetEntityWithName(targetName);
			myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(myPlayer);
			if (target != nullptr)
			{
				myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(target);
			}
		}
	}

	myScene->SetCamera(myCamera);

	for (int i = 0; i < myDirectionalLights.Size(); ++i)
	{
		myScene->AddLight(myDirectionalLights[i]);
	}

	for (int i = 0; i < myPointLights.Size(); ++i)
	{
		myScene->AddLight(myPointLights[i]);
	}

	for (int i = 0; i < mySpotLights.Size(); ++i)
	{
		myScene->AddLight(mySpotLights[i]);
	}

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myScene->AddInstance(gfxComp->GetInstance());
		}
	}
	myRenderStuff = true;
}


Level::~Level()
{
	delete myCamera;
	delete myScene;
	myEntities.DeleteAll();

	delete mySkySphere;
	delete myEntityFactory;
	delete myWeaponFactory;
	delete myBulletManager;
	delete myCollisionManager;
	delete myMissionManager;

	myDirectionalLights.DeleteAll();
	myPointLights.DeleteAll();
	mySpotLights.DeleteAll();
}

void Level::SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName)
{
	Prism::ModelProxy* skySphere = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		aModelFilePath, aEffectFileName);
	delete mySkySphere;
	mySkySphere = new Prism::Instance(*skySphere, Prism::eOctreeType::NOT_IN_OCTREE);
}

bool Level::LogicUpdate(float aDeltaTime)
{
	myCollisionManager->CleanUp();

	if (myPlayer->GetAlive() == false)
	{
		return true;
	}

	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->GetAlive() == false)
		{
			GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

			if (gfxComp != nullptr)
			{
				myScene->RemoveInstance(gfxComp->GetInstance());
			}

			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(aDeltaTime);

		/*if (myEntities[i]->GetType() == eEntityType::POWERUP)
		{
			myPlayer->SendNote<WaypointNote>(WaypointNote(myEntities[i]->myOrientation.GetPos()));
		}*/

		if (myEntities[i]->GetType() == eEntityType::ENEMY)
		{
			myPlayer->SendNote<GUINote>(GUINote(myEntities[i]->myOrientation.GetPos(), eGUINoteType::ENEMY));
		}
	}

	UpdateDebug();

	myCollisionManager->Update();
	myBulletManager->Update(aDeltaTime);
	myMissionManager->Update(aDeltaTime);
	mySkySphere->SetPosition(myCamera->GetOrientation().GetPos());
	myCamera->Update();
	return myComplete;
}

void Level::Render()
{
	Prism::Engine::GetInstance()->DisableZBuffer();
	mySkySphere->Render(*myCamera);
	Prism::Engine::GetInstance()->EnableZBuffer();

	if (myRenderStuff)
	{
		myScene->Render(myBulletManager->GetInstances());
	}

	myPlayer->GetComponent<GUIComponent>()->Render(Prism::Engine::GetInstance()->GetWindowSize(), myInputWrapper->GetMousePosition());


	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().x), CU::Vector2<float>(0, 0));
	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().y), CU::Vector2<float>(0, -30));
	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().z), CU::Vector2<float>(0, -60));

	Prism::Engine::GetInstance()->PrintDebugText(std::to_string(myPlayer->GetComponent<HealthComponent>()->GetHealth()), { 0, -100.f });
}


void Level::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

Entity* Level::AddTrigger(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	Entity* newEntity = new Entity(eEntityType::TRIGGER, *myScene, Prism::eOctreeType::NOT_IN_OCTREE);
	float entityRadius;
	aReader.ForceReadAttribute(aElement, "radius", entityRadius);
	myEntityFactory->CopyEntity(newEntity, "trigger");

	newEntity->GetComponent<CollisionComponent>()->SetRadius(entityRadius);

	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "position");
	CU::Vector3<float> triggerPosition;
	aReader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
	aReader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
	aReader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
	newEntity->myOrientation.SetPos(triggerPosition*10.f);

	myEntities.Add(newEntity);
	myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::TRIGGER);

	return myEntities.GetLast();
}

void Level::ReadXML(const std::string& aFile)
{
	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFile);
	tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("root");
	levelElement = reader.ForceFindFirstChild(levelElement, "scene");
	std::string skySphere;
	std::string cubeMap;
	std::string missionXML;

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "missionxml"), "source", missionXML);

	myMissionManager = new MissionManager(*this, *myPlayer, missionXML);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "skysphere"), "source", skySphere);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "cubemap"), "source", cubeMap);
	
	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap(cubeMap);

	SetSkySphere(skySphere, "Data/effect/SkyboxEffect.fx");

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
		myDirectionalLights.Add(newDirLight);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "enemy"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "enemy"))
	{
		Entity* newEntity = new Entity(eEntityType::ENEMY, *myScene, Prism::eOctreeType::DYNAMIC);
		std::string enemyType;
		reader.ForceReadAttribute(entityElement, "enemyType", enemyType);
		myEntityFactory->CopyEntity(newEntity, enemyType);

		tinyxml2::XMLElement* enemyElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> enemyPosition;
		reader.ForceReadAttribute(enemyElement, "X", enemyPosition.x);
		reader.ForceReadAttribute(enemyElement, "Y", enemyPosition.y);
		reader.ForceReadAttribute(enemyElement, "Z", enemyPosition.z);
		newEntity->myOrientation.SetPos(enemyPosition*10.f);

		enemyElement = reader.ForceFindFirstChild(entityElement, "rotation");
		CU::Vector3<float> enemyRotation;
		reader.ForceReadAttribute(enemyElement, "X", enemyRotation.x);
		reader.ForceReadAttribute(enemyElement, "Y", enemyRotation.y);
		reader.ForceReadAttribute(enemyElement, "Z", enemyRotation.z);

		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundX(enemyRotation.x) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundY(enemyRotation.y) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundZ(enemyRotation.z) * newEntity->myOrientation;

		int health = 0;
		reader.ForceReadAttribute(entityElement, "hp", health);
		newEntity->AddComponent<HealthComponent>()->Init(health);
		newEntity->AddComponent<CollisionComponent>()->Initiate(7.5f);
		myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::ENEMY);

		myEntities.Add(newEntity);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "prop"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity(eEntityType::PROP, *myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		reader.ForceReadAttribute(entityElement, "propType", propType);
		myEntityFactory->CopyEntity(newEntity, propType);

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

		newEntity->AddComponent<PropComponent>();

		myEntities.Add(newEntity);
		myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::PROP);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "trigger"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "trigger"))
	{
		AddTrigger(reader, entityElement);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "powerup"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myScene, Prism::eOctreeType::STATIC);
		float entityRadius;

		tinyxml2::XMLElement* triggerElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> triggerPosition;
		reader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
		reader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
		reader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
		newEntity->myOrientation.SetPos(triggerPosition*10.f);


		triggerElement = reader.ForceFindFirstChild(entityElement, "type");
		std::string powerUp;
		reader.ForceReadAttribute(triggerElement, "powerup", powerUp);
		//CU::ToLower(powerUp);

		std::string powerType = CU::GetSubString(CU::ToLower(powerUp).c_str(), '_', true);

		//std::string powerType = CU::GetSubString(tempString, '_', false);


		if (powerType == "healthkit")
		{
			newEntity->SetPowerUp(ePowerUpType::HEALTHKIT);
		}
		if (powerType == "shield")
		{
			newEntity->SetPowerUp(ePowerUpType::SHIELDBOOST);
		}
		if (powerType == "firerate")
		{
			newEntity->SetPowerUp(ePowerUpType::FIRERATEBOOST);
		}

		myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myPlayer);
		myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);

		//newEntity->AddComponent<PowerUpComponent>()->Init(newEntity->GetPowerUpType());



		myEntities.Add(newEntity);
	}
}

Entity* Level::GetEntityWithName(const std::string& aName)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (CU::ToLower(myEntities[i]->GetName()) == CU::ToLower(aName))
		{
			return myEntities[i];
		}
	}
	return nullptr;
}

int Level::GetEnemiesAlive() const
{
	return myCollisionManager->GetEnemiesAlive();
}

void Level::LoadPlayer()
{
	Entity* player = new Entity(eEntityType::PLAYER, *myScene, Prism::eOctreeType::DYNAMIC);
	player->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx"
		, "Data/effect/NoTextureEffect.fx");
	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<ShootingComponent>();
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("machineGun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("sniperGun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("plasmaGun"));
	player->GetComponent<ShootingComponent>()->SetCurrentWeaponID(0);
	player->AddComponent<CollisionComponent>()->Initiate(7.5f);

	XMLReader reader;
	reader.OpenDocument("Data/script/player.xml");
	int health = 0;
	bool invulnerable = false;
	reader.ReadAttribute(reader.FindFirstChild("life"), "value", health);
	reader.ReadAttribute(reader.FindFirstChild("life"), "invulnerable", invulnerable);

	player->AddComponent<HealthComponent>()->Init(health, invulnerable);
	myCollisionManager->Add(player->GetComponent<CollisionComponent>(), eEntityType::PLAYER);

	myPlayer = player;
	myEntities.Add(player);
	myCamera = new Prism::Camera(player->myOrientation);
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);
	float maxMetersToEnemies = 0;
	reader.ReadAttribute(reader.ForceFindFirstChild("maxdistancetoenemiesinGUI"), "meters", maxMetersToEnemies);

	player->GetComponent<GUIComponent>()->Init(maxMetersToEnemies);
}

void Level::CompleteLevel()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::COMPLETE_LEVEL));
}

void Level::UpdateDebug()
{
	if (myInputWrapper->KeyDown(DIK_N) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(10);
	}
	if (myInputWrapper->KeyDown(DIK_M) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability(false);
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(10000000);
	}
	if (myInputWrapper->KeyDown(DIK_V) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability(!myPlayer->GetComponent<HealthComponent>()->GetInvulnerability());
	}
	if (myInputWrapper->KeyDown(DIK_B) == true)
	{
		CompleteLevel();
	}
	if (myInputWrapper->KeyDown(DIK_C))
	{
		PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::RELOAD_LEVEL));
	}
	if (myInputWrapper->KeyDown(DIK_P))
	{
		Prism::Engine::GetInstance()->ToggleWireframe();
	}
}