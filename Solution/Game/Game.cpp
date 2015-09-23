#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
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
#include "InputComponent.h"
#include <InputWrapper.h>
#include <Instance.h>
#include <Model.h>
#include <PointLight.h>
#include <Scene.h>
#include "ShootingComponent.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include "PostMaster.h"

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
	myLight->SetColor({ 1.f, 1.f, 0.f, 1.f });
	myLight->SetDir({ 0.f, 0.5f, -1.f });

	myPointLight = new Prism::PointLight();
	myPointLight->SetColor({ 1.f, 1.f, 1.f, 1.f });
	myPointLight->SetPosition({ 0, 0, 0, 0 });
	myPointLight->SetRange(50.f);
	myPointLight->Initiate();

	myEntities.Init(4);
	
	Entity* player = new Entity();
	
	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<GraphicsComponent>()->InitCube(10, 10, 10);
	player->AddComponent<ShootingComponent>()->Init();

	myEntities.Add(player);
	myCamera = new Prism::Camera(player->GetComponent<GraphicsComponent>()->GetInstance()->GetOrientation());
	player->myCamera = myCamera;
	mySkyboxModel = new Prism::Model();
	mySkyboxModel->InitSkyblox(500, 500, 500);
	mySkybox = new Prism::Instance(*mySkyboxModel);
	mySkybox->SetPosition(myCamera->GetOrientation().GetPos());

	for (int i = 0; i < 50; ++i)
	{
		Entity* astroids = new Entity();
		astroids->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Enemys/SM_Enemy_Ship_A.fbx",
			"Data/effect/BasicEffect.fx");
		astroids->GetComponent<GraphicsComponent>()->SetPosition({ static_cast<float>(rand() % 200 - 100), 
				static_cast<float>(rand() % 200 - 100), static_cast<float>(rand() % 200 - 100) });
		//astroids->GetComponent<GraphicsComponent>()->SetPosition({ 0.f, 10.f, 100.f });
		
		astroids->AddComponent<AIComponent>()->Init();
		astroids->GetComponent<AIComponent>()->SetEntityToFollow(player);
		
		myEntities.Add(astroids);
	}


	

	myScene = new Prism::Scene();
	myScene->SetCamera(myCamera);
	myScene->AddLight(myPointLight);
	myScene->AddInstance(mySkybox);

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myScene->AddInstance(gfxComp->GetInstance());
		}
	}

	myScene->AddLight(myLight);

	myRenderStuff = true;

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Audio/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Audio/SpaceShooterBank.bnk");

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
	if (deltaTime > 1.0f/10.0f)
	{
		deltaTime = 1.0f / 10.0f;
	}


	if (myInputWrapper->KeyDown(DIK_F5))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay()->ToggleFunctionTimers();
	}
	else if (myInputWrapper->KeyDown(DIK_F6))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay()->ToggleMemoryUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F7))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay()->ToggleCPUUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F8))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay()->ToggleFrameTime();
	}
	else if (myInputWrapper->KeyDown(DIK_F9))
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

	//float R = (rand()% 255+1);
	//float G = (rand()% 255+1);
	//float B = (rand()% 255+1);
	//
	//myLight->SetColor({ R/255.f, G/255.f, B/255.f, 1.f });

	mySkybox->SetPosition(myCamera->GetOrientation().GetPos());

	myBulletManager->Update(deltaTime);

	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();

	END_TIME_BLOCK("Game::Update");

	Render();
	

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
	myLight->PerformRotation(CU::Matrix33<float>::CreateRotateAroundX(globalPi / 4.f * aDeltaTime));

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		myEntities[i]->Update(aDeltaTime);
	}
}

void Game::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	BEGIN_TIME_BLOCK("Game::Render");

	if (myRenderStuff)
	{
		myScene->Render();
	}

	if (myShowPointLightCube == true)
	{
		myPointLight->Render(myCamera);
	}

	myBulletManager->Render(myCamera);

	END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay()->Render();

	VTUNE_EVENT_END();
}

