#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "DirectionalLight.h"
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
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "PointLight.h"
#include <Scene.h>
#include "ShootingComponent.h"
#include <sstream>
#include <string>
#include <XMLReader.h>


Level::Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper, BulletManager* aBulletManager, bool aShouldTestXML)
{
	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap("Data/resources/texture/cubemapTest.dds");
	myScene = new Prism::Scene();
	myEntityFactory = new EntityFactory();
	myEntityFactory->LoadEntites("Data/entities/EntityList.xml");
	myInputWrapper = aInputWrapper;
	myBulletManager = aBulletManager;
	myShowPointLightCube = false;
	myLight = new Prism::DirectionalLight();
	myLight->SetColor({ 0.5f, 0.5f, 0.5f, 1.f });
	myLight->SetDir({ 0.f, 0.5f, -1.f });

	myEntities.Init(4);

	Entity* player = new Entity();

	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<ShootingComponent>();
	player->AddComponent<CollisionComponent>()->Initiate(0);
	player->AddComponent<HealthComponent>()->Init(100);

	myPlayer = player;
	myEntities.Add(player);
	myCamera = new Prism::Camera(player->myOrientation);
	player->myCamera = myCamera;
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);

	SetSkySphere("Data/resources/model/skybox/skySphere_test.fbx", "Data/effect/SkyboxEffect.fx");
	if (aShouldTestXML == false)
	{
		for (int i = 0; i < 220; ++i)
		{
			Entity* astroids = new Entity();
			//astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
			//	"Data/effect/NoTextureEffect.fx");

			astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
				"Data/effect/NoTextureEffect.fx");
			//astroids->AddComponent<CollisionComponent>()->Initiate(15);

			astroids->GetComponent<GraphicsComponent>()->SetPosition({ static_cast<float>(rand() % 400 - 200)
				, static_cast<float>(rand() % 400 - 200), static_cast<float>(rand() % 400 - 200) });


			//astroids->AddComponent<AIComponent>()->Init();
			//astroids->GetComponent<AIComponent>()->SetEntityToFollow(player);

			myEntities.Add(astroids);
		}

		//for (int i = 0; i < 50; ++i)
		//{
		//	Entity* enemy = new Entity();
		//	//enemy->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
		//	//	"Data/effect/NoTextureEffect.fx");
		//	myEntityFactory->CopyEntity(enemy, "defaultEnemy");
		//
		//	enemy->GetComponent<GraphicsComponent>()->SetPosition({ static_cast<float>(rand() % 150 - 50),
		//		static_cast<float>(rand() % 200 - 100), static_cast<float>(rand() % 150 - 50) });
		//
		//	myEntities.Add(enemy);
		//}
	}
	else
	{
		WATCH_FILE(aFileName, Level::ReadXML);

		ReadXML(aFileName);
	}

	myCockPit = new Entity();
	//myCockPit->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx",
	//	"Data/effect/NoTextureEffect.fx");
	//myCockPit->GetComponent<GraphicsComponent>()->SetPosition({ 0,0, -10 });
	myCockPit->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx"
		, "Data/effect/NoTextureEffect.fx");
	myCockPit->GetComponent<GraphicsComponent>()->GetInstance()->SetOrientationPointer(myPlayer->myOrientation);
	myCockPit->AddComponent<CollisionComponent>()->Initiate(0);
	myEntities.Add(myCockPit);
	myScene->SetCamera(myCamera);
	myScene->AddLight(myLight);

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myScene->AddInstance(gfxComp->GetInstance());
		}
	}

	myRenderStuff = true;

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Audio/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Audio/SpaceShooterBank.bnk");

}


Level::~Level()
{
	delete myCamera;
	delete myScene;
	myEntities.DeleteAll();
}

void Level::SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName)
{
	Prism::ModelProxy* skySphere = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		aModelFilePath, aEffectFileName);

	mySkySphere = new Prism::Instance(*skySphere);
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

	if (myShowPointLightCube == true)
	{
		myPointLight->Render(myCamera);
	}

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i]->GetComponent<CollisionComponent>() != nullptr)
		{
			myEntities[i]->GetComponent<CollisionComponent>()->Render(myCamera);
		}
	}

	myPlayer->GetComponent<GUIComponent>()->Render(Prism::Engine::GetInstance()->GetWindowSize(), myInputWrapper->GetMousePosition());


	std::stringstream ss;
	std::stringstream ss2;
	std::stringstream ss3;

	ss << "X" << myPlayer->myOrientation.GetPos().x;
	ss2 << "Y" << myPlayer->myOrientation.GetPos().y;
	ss3 << "Z" << myPlayer->myOrientation.GetPos().z;
	Prism::Engine::GetInstance()->PrintDebugText(ss.str().c_str(), CU::Vector2<float>(0, 0));
	Prism::Engine::GetInstance()->PrintDebugText(ss2.str().c_str(), CU::Vector2<float>(0, -30));
	Prism::Engine::GetInstance()->PrintDebugText(ss3.str().c_str(), CU::Vector2<float>(0, -60));

	Prism::Engine::GetInstance()->PrintDebugText(std::to_string(myPlayer->GetComponent<HealthComponent>()->GetHealth()), { 0, -100.f });
}

void Level::LogicUpdate(float aDeltaTime)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		myEntities[i]->Update(aDeltaTime);
	}

	if (CheckCollision() == true)
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(1);
	}
	mySkySphere->SetPosition(myCamera->GetOrientation().GetPos());
}

void Level::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

bool Level::CheckCollision()
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i] != myPlayer)
		{
			continue;
		}

		for (int j = 0; j < myEntities.Size(); ++j)
		{
			if (myEntities[i]->GetComponent<CollisionComponent>() != nullptr && myEntities[j]->GetComponent<CollisionComponent>() != nullptr)
			{
				if (CommonUtilities::Intersection::SphereVsSphere
					(myEntities[i]->GetComponent<CollisionComponent>()->GetSphere()
					, myEntities[j]->GetComponent<CollisionComponent>()->GetSphere()) == true)
				{
					return true;
				}
				if (CommonUtilities::Intersection::SphereVsSphere
					(myEntities[j]->GetComponent<CollisionComponent>()->GetSphere()
					, myEntities[i]->GetComponent<CollisionComponent>()->GetSphere()) == true)
				{

					return true;
				}
			}
		}
	}
	return false;
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
		Prism::DirectionalLight* newDirLight = new Prism::DirectionalLight();

		CU::Vector3<float> lightDirection;
		reader.ForceReadAttribute(entityElement, "directionX", lightDirection.x);
		reader.ForceReadAttribute(entityElement, "directionY", lightDirection.y);
		reader.ForceReadAttribute(entityElement, "directionZ", lightDirection.z);
		newDirLight->SetDir(lightDirection);

		CU::Vector4<float> lightColor;
		reader.ForceReadAttribute(entityElement, "colourR", lightColor.myR);
		reader.ForceReadAttribute(entityElement, "colourG", lightColor.myG);
		reader.ForceReadAttribute(entityElement, "colourB", lightColor.myB);
		reader.ForceReadAttribute(entityElement, "colourA", lightColor.myA);
		newDirLight->SetColor(lightColor);

		myScene->AddLight(newDirLight);
	}


	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "enemy"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "enemy"))
	{
		Entity* newEntity = new Entity();
		std::string enemyType;
		reader.ForceReadAttribute(entityElement, "enemyType", enemyType);
		myEntityFactory->CopyEntity(newEntity, enemyType);

		CU::Vector3<float> entityPosition;
		reader.ForceReadAttribute(entityElement, "positionX", entityPosition.x);
		reader.ForceReadAttribute(entityElement, "positionY", entityPosition.y);
		reader.ForceReadAttribute(entityElement, "positionZ", entityPosition.z);

		newEntity->myOrientation.SetPos(entityPosition*10.f);

		myEntities.Add(newEntity);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "prop"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity();
		std::string propType;
		reader.ForceReadAttribute(entityElement, "propType", propType);
		myEntityFactory->CopyEntity(newEntity, propType);

		CU::Vector3<float> entityPosition;
		reader.ForceReadAttribute(entityElement, "positionX", entityPosition.x);
		reader.ForceReadAttribute(entityElement, "positionY", entityPosition.y);
		reader.ForceReadAttribute(entityElement, "positionZ", entityPosition.z);

		newEntity->myOrientation.SetPos(entityPosition*10.f);

		myEntities.Add(newEntity);
	}


	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "trigger"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "trigger"))
	{
		Entity* newEntity = new Entity();
		float entityRadius;
		reader.ForceReadAttribute(entityElement, "radius", entityRadius);
		myEntityFactory->CopyEntity(newEntity, "trigger");

		newEntity->GetComponent<CollisionComponent>()->SetRadius(entityRadius);

		CU::Vector3<float> entityPosition;
		reader.ForceReadAttribute(entityElement, "positionX", entityPosition.x);
		reader.ForceReadAttribute(entityElement, "positionY", entityPosition.y);
		reader.ForceReadAttribute(entityElement, "positionZ", entityPosition.z);

		newEntity->myOrientation.SetPos(entityPosition*10.f);

		myEntities.Add(newEntity);
	}

}
