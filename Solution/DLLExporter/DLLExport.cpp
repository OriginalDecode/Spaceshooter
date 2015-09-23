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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND locEngineWindowHandler;
Prism::SetupInfo locWindowSetup;
Prism::Scene locScene;
Prism::Model* locModel;
Prism::Instance* locInstance;
Prism::Camera* locCamera;
Prism::DirectionalLight* locDirectionLight;
CU::Matrix44f locPlayerPos;

Game* locGame = nullptr;

void StartEngine(int* aHwnd)
{
	DL_Debug::Debug::Create();
	CU::TimerManager::Create();

	HWND windowHandler = (HWND)aHwnd;

	Prism::Engine::Create(locEngineWindowHandler, WndProc, locWindowSetup);

	locDirectionLight = new Prism::DirectionalLight();
	locDirectionLight->SetDir({ 0.f, 0.5f, -1.f });
	locDirectionLight->SetColor({ 0.3f, 0.5f, 0.3f, 1.f });

	locPlayerPos.SetPos({ 0.f, 0.f, -5.f, 1.f });
	locCamera = new Prism::Camera(locPlayerPos);
	locScene.SetCamera(locCamera);
	locScene.AddLight(locDirectionLight);

	SetParent(locEngineWindowHandler, windowHandler);
	SetWindowLong(locEngineWindowHandler, GWL_STYLE, WS_POPUP);
	SetWindowPos(locEngineWindowHandler, HWND_TOP, 0, 0, locWindowSetup.myScreenWidth, locWindowSetup.myScreenHeight, SWP_SHOWWINDOW);
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
	//locGame->Update();
}

void LoadModel(const char* aModelFile, const char* aEffectFile)
{
	if (aEffectFile == "")
	{
		aEffectFile = "Data/effect/BasicEffect.fx";
	}
	locModel = Prism::Engine::GetInstance()->LoadModel(aModelFile, Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectFile));

	if (locInstance != nullptr) 
	{
		//MessageBox(NULL, "locInstance is now nullpointer and will be removed from locScene", "MSG", MB_OK);
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