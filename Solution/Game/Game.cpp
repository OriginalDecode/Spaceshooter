#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "ColoursForBG.h"
#include "Constants.h"
#include <DebugDataDisplay.h>
#include <DirectionalLight.h>
#include <EffectContainer.h>
#include "Entity.h"
#include <FileWatcher.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include <Instance.h>
#include <Intersection.h>
#include <Model.h>
#include <PointLight.h>
#include <Scene.h>
#include "ShootingComponent.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include "PostMaster.h"
#include <Vector.h>

Game::Game()
{
	PostMaster::Create();
	Prism::Audio::AudioInterface::CreateInstance();
	myInputWrapper = new CU::InputWrapper();
	myBulletManager = new BulletManager;
	myShowPointLightCube = false;
	//ShowCursor(false);
}

Game::~Game()
{
	delete myCamera;
	delete myInputWrapper;
	delete myScene;
	delete myBulletManager;
	myEntities.DeleteAll();
	Prism::Audio::AudioInterface::Destroy();
	PostMaster::Destroy();
}

bool Game::Init(HWND& aHwnd)
{

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myLight = new Prism::DirectionalLight();
	myLight->SetColor({ 0.5f, 0.5f, 0.5f, 1.f });
	myLight->SetDir({ 0.f, 0.5f, -1.f });

	myEntities.Init(4);

	Entity* player = new Entity();

	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<ShootingComponent>()->Init();
	player->AddComponent<CollisionComponent>()->Initiate(0);

	myPlayer = player;
	myEntities.Add(player);
	myCamera = new Prism::Camera(player->myOrientation);
	player->myCamera = myCamera;
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);


	Prism::Model* skySphere = Prism::Engine::GetInstance()->LoadModel("Data/resources/model/skybox/skySphere_test.fbx",
		Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/effect/SkyboxEffect.fx"));

	mySkybox = new Prism::Instance(*skySphere);
	//mySkybox->SetPosition({ 0.f, 0.f, 10.f });
	//mySkybox->SetPosition(myCamera->GetOrientation().GetPos());

	for (int i = 0; i < 10; ++i)
	{
		Entity* astroids = new Entity();
		//astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
		//	"Data/effect/NoTextureEffect.fx");

		astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/asteroids/asteroid__large_placeholder.fbx",
			"Data/effect/BasicEffect.fx");
		astroids->AddComponent<CollisionComponent>()->Initiate(15);

		astroids->GetComponent<GraphicsComponent>()->SetPosition({ static_cast<float>(rand() % 200 - 100), 
				static_cast<float>(rand() % 200 - 100), static_cast<float>(rand() % 200 - 100) });


		//astroids->AddComponent<AIComponent>()->Init();
		//astroids->GetComponent<AIComponent>()->SetEntityToFollow(player);

		myEntities.Add(astroids);
	}

	for (int i = 0; i < 50; ++i)
	{
		Entity* enemy = new Entity();
		enemy->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
			"Data/effect/NoTextureEffect.fx");


		enemy->GetComponent<GraphicsComponent>()->SetPosition({ static_cast<float>(rand() % 150 - 50),
			static_cast<float>(rand() % 200 - 100), static_cast<float>(rand() % 150 - 50) });


		enemy->AddComponent<AIComponent>()->Init();

		int chanceToFollowPlayer = rand() % 100;

		if (chanceToFollowPlayer > 75)
		{
			enemy->GetComponent<AIComponent>()->SetEntityToFollow(player);
		}
			
		enemy->AddComponent<CollisionComponent>()->Initiate(3);
		myEntities.Add(enemy);
	}

	myCockPit = new Entity();
	//myCockPit->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx",
	//	"Data/effect/NoTextureEffect.fx");
	//myCockPit->GetComponent<GraphicsComponent>()->SetPosition({ 0,0, -10 });
	myCockPit->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Player/SM_Cockpit.fbx",
		"Data/effect/NoTextureEffect.fx");
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


	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap("Data/resources/texture/un_Milkyway_test_cubemap.dds");

	Prism::Engine::GetInstance()->SetClearColor({ MAGENTA });

	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	Prism::Audio::AudioInterface::GetInstance()->Update();
	BEGIN_TIME_BLOCK("Game::Update");
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	if (deltaTime > 1.0f / 10.0f)
	{
		deltaTime = 1.0f / 10.0f;
	}
	//myCockPit->myOrientation = myPlayer->myOrientation;


	if (myInputWrapper->KeyDown(DIK_F9))
	{
		myShowPointLightCube = !myShowPointLightCube;
	}
	else if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myRenderStuff = !myRenderStuff;
	}
	if (myInputWrapper->KeyDown(DIK_P))
	{
		Prism::Engine::GetInstance()->ToggleWireframe();
	}

	LogicUpdate(deltaTime);
		std::stringstream ss;
	if (CheckCollision() == true)
	{
		myPlayer->myOrientation.SetPos(CU::Vector4<float>(10, 10, 10, 1));
	}

	mySkybox->SetPosition(myCamera->GetOrientation().GetPos());

	myBulletManager->Update(deltaTime);

	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();

	END_TIME_BLOCK("Game::Update");


	Render();


	Prism::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	Prism::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(deltaTime);
	return true;
}

void Game::Pause()
{

}

void Game::UnPause()
{

}

void Game::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

void Game::LogicUpdate(const float aDeltaTime)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		myEntities[i]->Update(aDeltaTime);
	}
}

void Game::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	BEGIN_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->DisableZBuffer();
	mySkybox->Render(*myCamera);
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
	ss3	<< "Z" << myPlayer->myOrientation.GetPos().z;
	Prism::Engine::GetInstance()->PrintDebugText(ss.str().c_str(), CU::Vector2<float>(0, 0));
	Prism::Engine::GetInstance()->PrintDebugText(ss2.str().c_str(), CU::Vector2<float>(0, -30));
	Prism::Engine::GetInstance()->PrintDebugText(ss3.str().c_str(), CU::Vector2<float>(0, -60));






		END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay()->Render();

	VTUNE_EVENT_END();
}

bool Game::CheckCollision()
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		for (int j = 0; j < myEntities.Size(); ++j)
		{
			if (CommonUtilities::Intersection::SphereVsSphere
				(myEntities[i]->GetComponent<CollisionComponent>()->GetSphere()
				, myEntities[j]->GetComponent<CollisionComponent>()->GetSphere()) == true)
			{
				if (myEntities[i] == myPlayer || myEntities[j] == myPlayer)
				{
					return true;
				}
			}
		}
	}
	return false;
}
