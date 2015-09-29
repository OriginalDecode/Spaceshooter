#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include <CommonHelper.h>
#include "DirectionalLight.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "Entity.h"
#include "EntityFactory.h"
#include <FileWatcher.h>
#include "GraphicsComponent.h"
#include "GUIComponent.h"
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

	myPlayer = player;
	myEntities.Add(player);
	myCamera = new Prism::Camera(player->myOrientation);
	player->myCamera = myCamera;
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);

	SetSkySphere("Data/resources/model/skybox/skySphere_test.fbx", "Data/effect/SkyboxEffect.fx");
	if (aShouldTestXML == false)
	{
		for (int i = 0; i < 1; ++i)
		{
			Entity* astroids = new Entity();
			//astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
			//	"Data/effect/NoTextureEffect.fx");

			astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
				"Data/effect/NoTextureEffect.fx");
			//astroids->AddComponent<CollisionComponent>()->Initiate(15);

			astroids->GetComponent<GraphicsComponent>()->SetPosition({ 30.f, 0.f, 100.f });


			astroids->AddComponent<AIComponent>()->Init();
			astroids->GetComponent<AIComponent>()->SetEntityToFollow(player);

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
	myScene = new Prism::Scene();
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
}

void Level::LogicUpdate(float aDeltaTime)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		myEntities[i]->Update(aDeltaTime);
	}

	//if (CheckCollision() == true)
	//{
	//	myPlayer->myOrientation.SetPos(CU::Vector4<float>(10, 10, 10, 1));
	//}
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
		for (int j = 0; j < myEntities.Size(); ++j)
		{
			if (CommonUtilities::Intersection::SphereVsSphere
				(myEntities[i]->GetComponent<CollisionComponent>()->GetSphere()
				, myEntities[j]->GetComponent<CollisionComponent>()->GetSphere()) == true)
			{
				if (myEntities[i] == myPlayer)
				{
					return true;
				}

			}
			if (CommonUtilities::Intersection::SphereVsSphere
				(myEntities[j]->GetComponent<CollisionComponent>()->GetSphere()
				, myEntities[i]->GetComponent<CollisionComponent>()->GetSphere()) == true)
			{
				if (myEntities[i] == myPlayer)
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
	//tinyxml2::XMLElement* gameObjectsReader = reader.FindFirstChild(levelReader, "GameObjects");

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

		newEntity->myOrientation.SetPos(entityPosition);

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

		newEntity->myOrientation.SetPos(entityPosition);

		myEntities.Add(newEntity);
	}


}
