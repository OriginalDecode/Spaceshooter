#include "stdafx.h"

#include <DebugDataDisplay.h>
#include <DirectionalLight.h>
#include <Camera.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include <InputWrapper.h>
#include <Instance.h>
#include <Model.h>
#include <PointLight.h>
#include <Scene.h>
#include <Text.h>
#include <TimerManager.h>
#include <VTuneApi.h>


#include "Entity.h"
#include "GraphicsComponent.h"

Game::Game()
{
	myCamera = new Prism::Camera();
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

	myLight = new Prism::DirectionalLight();
	myLight->SetColor({ 1.f, 0.6f, 0.6f, 1.f });
	myLight->SetDir({ 0.f, 0.5f, -1.f });

	myPointLight = new Prism::PointLight();

	myPointLight->SetColor({ 1.f, 0.f, 0.f, 1.f });
	myPointLight->SetPosition({ 0.f, 5.f, 0.f, 1.f });
	myPointLight->SetRange(15.f);
	myInstances.Init(4);


	myWaveModel = Prism::Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Wave.fx"));
	myGravityModel = Prism::Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/GravityWell.fx"));
	myExtrudeModel = Prism::Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Extrude.fx"));
	myNormalModel = Prism::Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx"
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/BasicEffect.fx"));

	
	myInstances.Add(new Prism::Instance(*myWaveModel));
	myInstances.Add(new Prism::Instance(*myGravityModel));
	myInstances.Add(new Prism::Instance(*myExtrudeModel));
	myInstances.Add(new Prism::Instance(*myNormalModel));
	myInstances.GetLast()->SetPosition({ 0.f, 25.f, 0.f });


	myInstances[0]->SetPosition({ -15.f, 10.f, 0.f });
	myInstances[1]->SetPosition({ 0.f, 10.f, 0.f });
	//myInstances[2]->SetPosition({ 15.f, 0.f, 0.f });
	//myInstances[3]->SetPosition({ 0.f, 15.f, 0.f });

	Prism::MeshData worldMesh;
	Prism::GeometryGenerator::CreateGrid(500.f, 500.f, 100, 100, worldMesh);
	myGeometryModel = new Prism::Model();
	myGeometryModel->InitGeometry(worldMesh);
	//myGeometryModel->SetEffect(Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Extrude.fx"));
	myInstances.Add(new Prism::Instance(*myGeometryModel));

	myScene = new Prism::Scene();
	myScene->SetCamera(myCamera);

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
	Prism::BEGIN_TIME_BLOCK("Game::Update");

	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	Prism::Engine::GetInstance()->GetEffectContainer().Update(deltaTime);
	Prism::Engine::GetInstance()->GetDebugDisplay().RecordFrameTime(deltaTime);

	if (myInputWrapper->KeyDown(DIK_F5))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleFunctionTimers();
	}
	else if (myInputWrapper->KeyDown(DIK_F6))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleMemoryUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F7))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleCPUUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F8))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleFrameTime();
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myRenderStuff = !myRenderStuff;
	}

	LogicUpdate(deltaTime);

	Prism::END_TIME_BLOCK("Game::Update");

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

	Prism::BEGIN_TIME_BLOCK("Game::Render");

	if (myRenderStuff)
	{
		myScene->Render();
	}

	Prism::END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay().Render(*myCamera);

	VTUNE_EVENT_END();
}
