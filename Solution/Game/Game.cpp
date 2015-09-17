#include "stdafx.h"

#include <DebugDataDisplay.h>
#include <DirectionalLight.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include <InputWrapper.h>
#include <Instance.h>
#include "Player.h"
#include <PointLight.h>
#include <Scene.h>
#include <Text.h>
#include <TimerManager.h>
#include <VTuneApi.h>

Game::Game()
{
	myInputWrapper = new CU::InputWrapper();
	myPlayer = new Player(*myInputWrapper);
	myCamera = new Camera(myPlayer->GetOrientation());
	ShowCursor(false);
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

	myLight = new DirectionalLight();
	myLight->SetColor({ 1.f, 0.6f, 0.6f, 1.f });
	myLight->SetDir({ 0.f, 0.5f, -1.f });

	myPointLight = new PointLight();
	myPointLight->SetColor({ 1.f, 0.f, 0.f, 1.f });
	myPointLight->SetPosition({ 0.f, 5.f, 0.f, 1.f });
	myPointLight->SetRange(15.f);
	myInstances.Init(4);

	myWaveModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Wave.fx"));
	myGravityModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/GravityWell.fx"));
	myExtrudeModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Extrude.fx"));
	myNormalModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/BasicEffect.fx"));

	
	myInstances.Add(new Instance(*myWaveModel));
	myInstances.Add(new Instance(*myGravityModel));
	myInstances.Add(new Instance(*myExtrudeModel));
	myInstances.Add(new Instance(*myNormalModel));
	myInstances.GetLast()->SetPosition({ 0.f, 25.f, 0.f });


	myInstances[0]->SetPosition({ -15.f, 10.f, 0.f });
	myInstances[1]->SetPosition({ 0.f, 10.f, 0.f });
	//myInstances[2]->SetPosition({ 15.f, 0.f, 0.f });
	//myInstances[3]->SetPosition({ 0.f, 15.f, 0.f });

	MeshData worldMesh;
	GeometryGenerator::CreateGrid(500.f, 500.f, 100, 100, worldMesh);
	myGeometryModel = new Model();
	myGeometryModel->InitGeometry(worldMesh);
	//myGeometryModel->SetEffect(Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Extrude.fx"));
	myInstances.Add(new Instance(*myGeometryModel));

	myScene = new Scene();
	myScene->SetCamera(myCamera);
	for (int i = 0; i < myInstances.Size(); ++i)
		myScene->AddInstance(myInstances[i]);

	//myScene->AddLight(myLight);
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
	myLight->Update();
	myLight->PerformRotation(CU::Matrix33<float>::CreateRotateAroundY((45.f * aDeltaTime) * 3.14f / 180.f));

	//myInstances[3]->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundX((720 * aDeltaTime) * 3.14f / 180.f));
	//myInstances[3]->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY((720 * aDeltaTime) * 3.14f / 180.f));

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
	myPlayer->Update(aDeltaTime);
	//if (myInputWrapper->KeyIsPressed(DIK_W))
	//{
	//	myCamera->MoveForward(100.f * aDeltaTime);
	//}
	//if (myInputWrapper->KeyIsPressed(DIK_S))
	//{
	//	myCamera->MoveForward(-100.f * aDeltaTime);
	//}
	//if (myInputWrapper->KeyIsPressed(DIK_A))
	//{
	//	myCamera->MoveRight(-100.f * aDeltaTime);
	//}
	//if (myInputWrapper->KeyIsPressed(DIK_D))
	//{
	//	myCamera->MoveRight(100.f * aDeltaTime);
	//}
	//
	//if (myInputWrapper->KeyIsPressed(DIK_Q))
	//{
	//	myCamera->RotateZ(90.f * aDeltaTime);
	//}
	//if (myInputWrapper->KeyIsPressed(DIK_E))
	//{
	//	myCamera->RotateZ(-90.f * aDeltaTime);
	//}
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
