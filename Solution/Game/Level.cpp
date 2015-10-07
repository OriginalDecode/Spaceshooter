#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "DirectionalLight.h"
#include "EnemiesTargetNote.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include "GUIComponent.h"
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
#include "PowerUpComponent.h"
#include <Scene.h>
#include "ShootingComponent.h"
#include <sstream>
#include <string>
#include <SpotLight.h>
#include "WeaponFactory.h"
#include "WaypointNote.h"

#include <XMLReader.h>


Level::Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper)
	: myEntities(16)
	, myComplete(false)
	, mySkySphere(nullptr)
{
	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap("Data/resources/texture/cubemapTest.dds");
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

	SetSkySphere("Data/resources/model/skybox/skySphere_test.fbx", "Data/effect/SkyboxEffect.fx");
	
	LoadPlayer();

	WATCH_FILE(aFileName, Level::ReadXML);

	ReadXML(aFileName);

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
	myMissionManager = new MissionManager(*this, *myPlayer, "Data/level/level1Missions.xml");
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
	mySkySphere = new Prism::Instance(*skySphere);
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
			myPlayer->SendNote<EnemiesTargetNote>(EnemiesTargetNote(myEntities[i]->myOrientation.GetPos()));
		}
	}


	if (myInputWrapper->KeyDown(DIK_N))
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(10);
	}

	myCollisionManager->Update();
	myBulletManager->Update(aDeltaTime);
	myMissionManager->Update(aDeltaTime);
	mySkySphere->SetPosition(myCamera->GetOrientation().GetPos());

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

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i]->GetComponent<CollisionComponent>() != nullptr)
		{
			myEntities[i]->GetComponent<CollisionComponent>()->Render(myCamera);
		}
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
	Entity* newEntity = new Entity(eEntityType::TRIGGER, *myScene);
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
		Entity* newEntity = new Entity(eEntityType::ENEMY, *myScene);
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
		Entity* newEntity = new Entity(eEntityType::PROP, *myScene);
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

		myEntities.Add(newEntity);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "trigger"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "trigger"))
	{
		AddTrigger(reader, entityElement);
	}
	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "powerup"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myScene);
		float entityRadius;
		reader.ForceReadAttribute(entityElement, "radius", entityRadius);


		tinyxml2::XMLElement* triggerElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> triggerPosition;
		reader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
		reader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
		reader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
		newEntity->myOrientation.SetPos(triggerPosition*10.f);


		triggerElement = reader.ForceFindFirstChild(entityElement, "Type");
		std::string powerUp;
		reader.ForceReadAttribute(triggerElement, "powerup", powerUp);
		CU::ToLower(powerUp);


		if (powerUp == "healthkit_01")
		{
			newEntity->SetPowerUp(ePowerUpType::HEALTHKIT_01);
		}
		if (powerUp == "healthkit_02")
		{
			newEntity->SetPowerUp(ePowerUpType::HEALTHKIT_02);
		}
		if (powerUp == "shield")
		{
			newEntity->SetPowerUp(ePowerUpType::SHIELDBOOST);
		}
		if (powerUp == "firerate")
		{
			newEntity->SetPowerUp(ePowerUpType::FIRERATEBOOST);
		}

		myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<CollisionComponent>()->SetRadius(entityRadius);
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
	Entity* player = new Entity(eEntityType::PLAYER, *myScene);
	player->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx"
		, "Data/effect/NoTextureEffect.fx");
	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<ShootingComponent>();
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("machineGun"));
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
	player->myCamera = myCamera;
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);
}