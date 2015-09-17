#include "stdafx.h"

#include <DebugDataDisplay.h>
#include <DirectionalLight.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include <InputWrapper.h>
#include <Instance.h>
#include <PointLight.h>
#include <Scene.h>
#include <Text.h>
#include <TimerManager.h>
#include <VTuneApi.h>


#include "Entity.h"
#include "GraphicsComponent.h"

Game::Game()
{
	myCamera = new Camera();
	myInputWrapper = new CU::InputWrapper();
}

Game::~Game()
{
	delete myCamera;
	delete myInputWrapper;
	delete myScene;
	myInstances.DeleteAll();
}

bool Game::Init(HWND& aHwnd)
{
	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myPointLight = new PointLight();
	myPointLight->SetColor({ 1.f, 0.f, 0.f, 1.f });
	myPointLight->SetPosition({ 0.f, 5.f, 0.f, 1.f });
	myPointLight->SetRange(15.f);
	myInstances.Init(4);

	//Entity* newObject = new Entity();
	//newObject->AddComponent<GraphicsComponent>()->Init();
	//
	//myScene->AddInstance(newObject->GetComponent<GraphicsComponent>()->GetInstance());

	myScene = new Scene();
	for (int i = 0; i < myInstances.Size(); ++i)
		myScene->AddInstance(myInstances[i]);

	myScene->AddLight(myPointLight);

	myRenderStuff = true;


	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	BEGIN_TIME_BLOCK("Game::Update");

	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	Engine::GetInstance()->GetEffectContainer().Update(deltaTime);
	Engine::GetInstance()->GetDebugDisplay().RecordFrameTime(deltaTime);

	if (myInputWrapper->KeyDown(DIK_F5))
	{
		Engine::GetInstance()->GetDebugDisplay().ToggleFunctionTimers();
	}
	else if (myInputWrapper->KeyDown(DIK_F6))
	{
		Engine::GetInstance()->GetDebugDisplay().ToggleMemoryUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F7))
	{
		Engine::GetInstance()->GetDebugDisplay().ToggleCPUUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F8))
	{
		Engine::GetInstance()->GetDebugDisplay().ToggleFrameTime();
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myRenderStuff = !myRenderStuff;
	}

	LogicUpdate(deltaTime);

	END_TIME_BLOCK("Game::Update");

	Render();
	
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
	if (myInputWrapper->KeyIsPressed(DIK_UP))
	{
		myCamera->RotateX(-90.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_DOWN))
	{
		myCamera->RotateX(90.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_LEFT))
	{
		myCamera->RotateY(-90.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_RIGHT))
	{
		myCamera->RotateY(90.f * aDeltaTime);
	}

	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		myCamera->MoveForward(100.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		myCamera->MoveForward(-100.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_A))
	{
		myCamera->MoveRight(-100.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_D))
	{
		myCamera->MoveRight(100.f * aDeltaTime);
	}

	if (myInputWrapper->KeyIsPressed(DIK_Q))
	{
		myCamera->RotateZ(90.f * aDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_E))
	{
		myCamera->RotateZ(-90.f * aDeltaTime);
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

	END_TIME_BLOCK("Game::Render");

	Engine::GetInstance()->GetDebugDisplay().Render(*myCamera);

	VTUNE_EVENT_END();
}
