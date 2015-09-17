#include "stdafx.h"

#include "Engine.h"
#include "DebugDataDisplay.h"
#include "DirectX.h"
#include <DL_Debug.h>
#include "Model.h"
#include <TimerManager.h>
#include "TextureContainer.h"
#include "VTuneApi.h"


Prism::Engine* Prism::Engine::myInstance = nullptr;

Prism::Engine::Engine()
{
	myTextureContainer = new TextureContainer();
}

Prism::Engine::~Engine()
{
	delete myTextureContainer;
	myTextureContainer = nullptr;
}

bool Prism::Engine::Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo)
{
	myInstance = new Engine();
	myInstance->mySetupInfo = &aSetupInfo;
	return myInstance->Init(aHwnd, aWndProc);
}

Prism::Engine* Prism::Engine::GetInstance()
{
	return myInstance;
}

void Prism::Engine::Shutdown()
{
	myDirectX->CleanD3D();
	delete myDirectX;
	myDirectX = nullptr;
}

void Prism::Engine::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::RENDER);

	TIME_FUNCTION

	myDirectX->Present(0, 0);

	float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	myDirectX->Clear(clearColor);

	VTUNE_EVENT_END();
}

void Prism::Engine::OnResize(int aWidth, int aHeigth)
{
	myDirectX->OnResize(aWidth, aHeigth);
}

ID3D11Device* Prism::Engine::GetDevice()
{
	return myDirectX->GetDevice();
}

ID3D11DeviceContext* Prism::Engine::GetContex()
{
	return myDirectX->GetContex();
}

bool Prism::Engine::Init(HWND& aHwnd, WNDPROC aWndProc)
{
	TIME_FUNCTION

	if (WindowSetup(aHwnd, aWndProc) == false)
	{
		ENGINE_LOG("Failed to Create Window");
		return false;
	}

	myDirectX = new DirectX(aHwnd, *mySetupInfo);
	if (myDirectX == nullptr)
	{
		ENGINE_LOG("Failed to Setup DirectX");
		return false;
	}

	ShowWindow(aHwnd, 10);
	UpdateWindow(aHwnd);

	myDebugDataDisplay.Init();



	ENGINE_LOG("Engine Init Successful");
	return true;
}

Prism::Model* Prism::Engine::LoadModel(const std::string& aPath, Effect* aEffect)
{
	CU::TimerManager::GetInstance()->StartTimer("LoadModel");
	Model* model = myModelFactory.LoadModel(aPath.c_str(), aEffect);
	model->Init();


	int elapsed = static_cast<int>(CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());

	RESOURCE_LOG("Model \"%s\" took %d ms to load", aPath.c_str(), elapsed);

	return model;
}

void Prism::Engine::EnableZBuffer()
{
	myDirectX->EnableZBuffer();
}

void Prism::Engine::DisableZBuffer()
{
	myDirectX->DisableZBuffer();
}

bool Prism::Engine::WindowSetup(HWND& aHwnd, WNDPROC aWindowProc)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = aWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DirectX Window";
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

	if (RegisterClassEx(&wcex) == FALSE)
	{
		ENGINE_LOG("Failed to register WindowClass");
		return FALSE;
	}

	RECT rc = { 0, 0, mySetupInfo->myScreenWidth, mySetupInfo->myScreenHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	aHwnd = CreateWindow("DirectX Window", "DirectX Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (!aHwnd)
	{
		ENGINE_LOG("Failed to CreateWindow");
		return FALSE;
	}

	ENGINE_LOG("Window Setup Successful");
	return TRUE;
}