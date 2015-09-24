#pragma warning(disable : 4005)

#include "DLLExport.h"
#include <Engine.h>
#include <Windows.h>
#include <SetupInfo.h>
#include <DL_Debug.h>
#include <Game.h>
#include <TimerManager.h>
#include <Scene.h>
#include <Instance.h>
#include <Model.h>
#include <EffectContainer.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <TimerManager.h>
#include <InputWrapper.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND locEngineWindowHandler;
HWND locPanelWindowHandler;
Prism::SetupInfo locWindowSetup;
Prism::Scene locScene;
Prism::Model* locModel;
Prism::Instance* locInstance;
Prism::Camera* locCamera;
Prism::DirectionalLight* locDirectionLight;
CU::Matrix44f locPlayerPos;
CU::InputWrapper locInput;

Game* locGame = nullptr;

void StartEngine(int* aHwnd)
{
	DL_Debug::Debug::Create();
	CU::TimerManager::Create();

	locPanelWindowHandler = (HWND)aHwnd;

	Prism::Engine::Create(locEngineWindowHandler, WndProc, locWindowSetup);

	locDirectionLight = new Prism::DirectionalLight();
	locDirectionLight->SetDir({ 0.f, 0.5f, -1.f });
	locDirectionLight->SetColor({ 0.7f, 0.7f, 0.7f, 1.f });

	//locPlayerPos.SetPos({ 0.f, 0.f, -5.f, 1.f });
	locCamera = new Prism::Camera(locPlayerPos);
	locScene.SetCamera(locCamera);
	locScene.AddLight(locDirectionLight);

	locInput.Init(locPanelWindowHandler, GetModuleHandle(NULL),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	SetParent(locEngineWindowHandler, locPanelWindowHandler);
	SetWindowLong(locEngineWindowHandler, GWL_STYLE, WS_POPUP);
	SetWindowPos(locEngineWindowHandler, HWND_TOP, 0, 0, locWindowSetup.myScreenWidth, 
		locWindowSetup.myScreenHeight, SWP_SHOWWINDOW);
}

void SetupWindow(int aWidth, int aHeight)
{
	locWindowSetup.myMSAACount = 4;
	locWindowSetup.myScreenHeight = aHeight;
	locWindowSetup.myScreenWidth = aWidth;
	locWindowSetup.myWindowed = true;
}

void Render()
{
	Prism::Engine::GetInstance()->Render();
	locScene.Render();
}

void Update()
{
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	locInput.Update();
	if (locInput.KeyIsPressed(DIK_ADD) || locInput.GetMouseDZ() < 0)
	{
		locCamera->MoveForward(10.f * deltaTime);
	}
	if (locInput.KeyIsPressed(DIK_SUBTRACT) || locInput.GetMouseDZ() > 0)
	{
		locCamera->MoveForward(-10.f * deltaTime);
	}
	if (locInput.KeyIsPressed(DIK_D))
	{
		//locCamera->RotateY(10.f * deltaTime);
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY(1.f * deltaTime));
	}
	else if (locInput.KeyIsPressed(DIK_A))
	{
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY(-1.f * deltaTime));
	}
	if (locInput.KeyIsPressed(DIK_W))
	{
		//locCamera->RotateY(10.f * deltaTime);
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundX(1.f * deltaTime));
	}
	else if (locInput.KeyIsPressed(DIK_S))
	{
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundX(-1.f * deltaTime));
	}
	if (locInput.KeyIsPressed(DIK_Q))
	{
		//locCamera->RotateY(10.f * deltaTime);
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundZ(1.f * deltaTime));
	}
	else if (locInput.KeyIsPressed(DIK_E))
	{
		locInstance->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundZ(-1.f * deltaTime));
	}
}

void LoadModel(const char* aModelFile, const char* aEffectFile)
{
	if (aEffectFile == "")
	{
		aEffectFile = "Data/effect/BasicEffect.fx";
	}
	locModel = Prism::Engine::GetInstance()->LoadModel(aModelFile, 
		Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectFile));

	if (locInstance != nullptr) 
	{
		locScene.RemoveInstance(locInstance);
		delete locInstance;
		locInstance = nullptr;
	}

	locInstance = new Prism::Instance(*locModel);

	locScene.AddInstance(locInstance);
}

void SetEffect(const char* aEffectFile)
{
	if (locInstance != nullptr)
	{
		locInstance->SetEffect(aEffectFile);
	}
}

void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel)
{
	Prism::Engine::GetInstance()->SetClearColor({ aRChannel, aGChannel, aBChannel, aAChannel });
}

void DirectionaLightRotateX(float aXAngle)
{
	CU::Vector3<float> rotatedDirection(locDirectionLight->GetDir());
	rotatedDirection.myX = aXAngle;
	locDirectionLight->SetDir(rotatedDirection);
}

void DirectionaLightRotateY(float aYAngle)
{
	CU::Vector3<float> rotatedDirection(locDirectionLight->GetDir());
	rotatedDirection.myY = aYAngle;
	locDirectionLight->SetDir(rotatedDirection);
}

void DirectionaLightRotateZ(float aZAngle)
{
	CU::Vector3<float> rotatedDirection(locDirectionLight->GetDir());
	rotatedDirection.myZ = aZAngle;
	locDirectionLight->SetDir(rotatedDirection);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}