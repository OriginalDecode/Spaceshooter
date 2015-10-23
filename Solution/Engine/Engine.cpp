#include "stdafx.h"

#include "DebugDataDisplay.h"
#include "DirectX.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "FBXFactory.h"
#include "FileWatcher.h"
#include "FontContainer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include <TimerManager.h>
#include "Text.h"
#include "TextureContainer.h"
#include <Vector.h>
#include "VTuneApi.h"

namespace Prism
{
	Engine* Engine::myInstance = nullptr;

	Engine::Engine()
		: myClearColor({ 0.8f, 0.125f, 0.8f, 1.0f })
		, myDebugTexts(16)
	{
		myTextureContainer = new TextureContainer();
		myEffectContainer = new EffectContainer();
		myModelFactory = new FBXFactory();
		myFontContainer = new FontContainer();
		myDebugDataDisplay = new DebugDataDisplay();
		myFileWatcher = new FileWatcher();
		myModelLoader = new ModelLoader();

		myWireframeIsOn = false;
		myWireframeShouldShow = false;
	}

	Engine::~Engine()
	{
		delete myDebugText;
		myDebugText = nullptr;
		delete myTextureContainer;
		delete myEffectContainer;
		delete myModelFactory;
		delete myFontContainer;
		delete myDebugDataDisplay;
		delete myFileWatcher;

		Prism::Engine::GetInstance()->GetModelLoader()->ClearLoadJobs();
		while (Prism::Engine::GetInstance()->GetModelLoader()->IsLoading() == true)
		{
			//wait for ModelLoader to exit its loading-loop
		}
		myModelLoader->Shutdown();
		myModelLoaderThread->join();
		delete myModelLoader;

		delete myDirectX;
		myDirectX = nullptr;
	}

	bool Engine::Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo)
	{
		myInstance = new Engine();
		myInstance->mySetupInfo = &aSetupInfo;

		return myInstance->Init(aHwnd, aWndProc);
	}

	void Engine::Destroy()
	{
		delete myInstance;
		myInstance = nullptr;
	}

	Engine* Engine::GetInstance()
	{
		return myInstance;
	}

	void Engine::Render()
	{
		VTUNE_EVENT_BEGIN(VTUNE::RENDER);

		TIME_FUNCTION

		for (int i = 0; i < myDebugTexts.Size(); ++i)
		{
			myDebugText->Render(myDebugTexts[i].myText.c_str(), myDebugTexts[i].myPosition.x, myDebugTexts[i].myPosition.y
				, myDebugTexts[i].myScale);
		}
		myDebugTexts.RemoveAll();
		myDirectX->Present(0, 0);

#ifdef RELEASE_BUILD
#ifndef DLL_EXPORT
		myClearColor = { 0.f, 0.f, 0.f, 1.f };
#endif
#endif

		float clearColor[4] = { myClearColor.myR, myClearColor.myG, myClearColor.myB, myClearColor.myA };
		myDirectX->Clear(clearColor);

		VTUNE_EVENT_END();
	}

	void Engine::OnResize(int aWidth, int aHeigth)
	{
		myWindowSize.x = aWidth;
		myWindowSize.y = aHeigth;
		myDirectX->OnResize(aWidth, aHeigth);

		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(myWindowSize.x)
			, static_cast<float>(myWindowSize.y), 0.1f, 1000.f);
	}

	Model* Engine::DLLLoadModel(const std::string& aModelPath, Effect* aEffect)
	{
		CU::TimerManager::GetInstance()->StartTimer("LoadModel");

		Model* model = myModelFactory->LoadModel(aModelPath.c_str(), aEffect);
		model->Init();

		int elapsed = static_cast<int>(
			CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
		RESOURCE_LOG("Model \"%s\" took %d ms to load", aModelPath.c_str(), elapsed);

		return model;
	}

	ID3D11Device* Engine::GetDevice()
	{
		return myDirectX->GetDevice();
	}

	ID3D11DeviceContext* Engine::GetContex()
	{
		return myDirectX->GetContex();
	}

	bool Engine::Init(HWND& aHwnd, WNDPROC aWndProc)
	{
		myWindowSize.x = mySetupInfo->myScreenWidth;
		myWindowSize.y = mySetupInfo->myScreenHeight;

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

		myDebugDataDisplay->Init();


		myDebugText = new Text();
		myDebugText->Init(GetFontContainer()->GetFont("Data/Resource/Font/F_default.dds"));

		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(myWindowSize.x)
			, static_cast<float>(myWindowSize.y), 0.1f, 1000.f);

		myModelLoaderThread = new std::thread(&ModelLoader::Run, myModelLoader);

		ENGINE_LOG("Engine Init Successful");
		return true;
	}

	void Engine::PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale)
	{
		//myDebugText->Render(aText.c_str(), aPosition.x, aPosition.y, aScale);
		DebugText toAdd;
		toAdd.myText = aText;
		toAdd.myPosition = aPosition;
		toAdd.myScale = aScale;
		myDebugTexts.Add(toAdd);
	}

	void Engine::PrintDebugText(float aNumber, const CU::Vector2<float>& aPosition, float aScale)
	{
		std::stringstream ss;
		ss.precision(3);
		ss << aNumber;
		PrintDebugText(ss.str(), aPosition, aScale);
	}

	void Engine::PrintDebugText(int aNumber, const CU::Vector2<float>& aPosition, float aScale)
	{
		std::stringstream ss;
		ss.precision(3);
		ss << aNumber;
		PrintDebugText(ss.str(), aPosition, aScale);
	}

	void Engine::EnableZBuffer()
	{
		myDirectX->EnableZBuffer();
	}

	void Engine::DisableZBuffer()
	{
		myDirectX->DisableZBuffer();
	}

	void Engine::ToggleWireframe()
	{
		myDirectX->EnableWireframe();

		
		if (myWireframeIsOn == true)
		{
			myDirectX->DisableWireframe();
			myWireframeIsOn = false;
			myWireframeShouldShow = false;
			return;
		}

		myWireframeShouldShow = true;
		myWireframeIsOn = true;
	}

	void Engine::EnableWireframe()
	{
		myDirectX->EnableWireframe();
	}

	void Engine::DisableWireframe()
	{
		myDirectX->DisableWireframe();
	}

	bool Engine::WindowSetup(HWND& aHwnd, WNDPROC aWindowProc)
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

		aHwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			"DirectX Window",
			"DirectX Window",
			WS_OVERLAPPEDWINDOW,
			-1,
			-2,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL, 
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (!aHwnd)
		{
			ENGINE_LOG("Failed to CreateWindow");
			return FALSE;
		}

		if (mySetupInfo->myWindowed == false)
		{
			SetWindowLong(aHwnd, GWL_STYLE, WS_POPUP);
		}

		ENGINE_LOG("Window Setup Successful");
		return TRUE;
	}
}