#include "DLLApp.h"

#include <AudioInterface.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <DL_Debug.h>
#include <EffectContainer.h>
#include <Engine.h>
#include <EngineEnums.h>
#include <Entity.h>
#include <FileWatcher.h>
#include <GraphicsComponent.h>
#include <Instance.h>
#include <InputComponent.h>
#include <InputWrapper.h>
#include <PostMaster.h>
#include <Scene.h>
#include <SetupInfo.h>
#include <TimerManager.h>

#define AudioInstance Prism::Audio::AudioInterface::GetInstance()
#define EngineInstance Prism::Engine::GetInstance()

DLLApp::DLLApp(int* aHwnd, Prism::SetupInfo& aWindowSetup, WNDPROC aWindowProc)
{
	DL_Debug::Debug::Create();
	CU::TimerManager::Create();
	PostMaster::Create();
	Prism::Audio::AudioInterface::CreateInstance();
	AudioInstance->Init("Data/Resource/Sound/Init.bnk");
	AudioInstance->LoadBank("Data/Resource/Sound/SpaceShooterBank.bnk");

	myScene = new Prism::Scene();
	myPanelWindowHandler = (HWND)aHwnd;

	Prism::Engine::Create(myEngineWindowHandler, aWindowProc, aWindowSetup);

	SetupLight();
	SetupInput();
	SetParentWindow(aWindowSetup);
	SetupCubeMap();
	SetupObject();
	SetupCamera(aWindowSetup);
}

DLLApp::~DLLApp()
{
}

void DLLApp::Render()
{
	Prism::Engine::GetInstance()->Render();
	myScene->Render();
}

void DLLApp::Update()
{
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	myInput.Update();
	LogicUpdate(deltaTime);
}

void DLLApp::LoadModel(const char* aModelFile, const char* aShaderFile)
{
	if (aShaderFile == "")
	{
		aShaderFile = "Data/Resource/Shader/S_effect_basic.fx";
	}

	WatchCurrentFiles(aModelFile, aShaderFile);
	CU::Matrix44f currentOrientation = myObjectEntity->myOrientation;
	delete myObjectEntity;

	myObjectEntity = new Entity(eEntityType::PROP, *myScene, Prism::eOctreeType::DYNAMIC);
	myObjectEntity->AddComponent<InputComponent>()->Init(myInput);

	myObjectEntity->AddComponent<GraphicsComponent>()->InitDLL(aModelFile, aShaderFile);
	myObjectEntity->myOrientation = currentOrientation;
	GraphicsComponent* gfxComponent = myObjectEntity->GetComponent<GraphicsComponent>();
	gfxComponent->GetInstance()->SetEffect(aShaderFile);

	myScene->AddInstance(gfxComponent->GetInstance());
}

void DLLApp::SetClearColor(CU::Vector4f& aClearColor)
{
	Prism::Engine::GetInstance()->SetClearColor({ aClearColor.myR, aClearColor.myG, aClearColor.myB, aClearColor.myA });
}

void DLLApp::WatchCurrentFiles(const char* aModelFile, const char* aShaderFile)
{
	myModelFile = aModelFile;
	myShaderFile = aShaderFile;

	Prism::Engine::GetInstance()->GetFileWatcher()->Clear();
	Prism::Engine::GetInstance()->GetFileWatcher()->WatchFile(aModelFile, std::bind(&DLLApp::LoadModel, this, aModelFile, aShaderFile));
}

void DLLApp::LogicUpdate(float aDeltaTime)
{
	if (GetActiveWindow()) {
		if (myInput.KeyIsPressed(DIK_LALT) && myInput.MouseIsPressed(0))
		{
			CameraZoom(aDeltaTime);
		}
		if (myInput.KeyIsPressed(DIK_LALT) && myInput.MouseIsPressed(2))
		{
			CameraPan(aDeltaTime);
		}
		if (myInput.KeyIsPressed(DIK_LALT) && myInput.MouseIsPressed(1))
		{
			CameraRotation(aDeltaTime);
		}
	}
	AutoRotateObject(aDeltaTime);
}

void DLLApp::AutoRotateObject(float aDeltaTime)
{
	CU::Vector3f orginalPos(myObjectEntity->myOrientation.GetPos());
	myObjectEntity->myOrientation.SetPos(CU::Vector3f());
	myObjectEntity->GetComponent<InputComponent>()->RotateX(myAutoRotationSpeed.myX * aDeltaTime);
	myObjectEntity->GetComponent<InputComponent>()->RotateY(myAutoRotationSpeed.myY * aDeltaTime);
	myObjectEntity->GetComponent<InputComponent>()->RotateZ(myAutoRotationSpeed.myZ * aDeltaTime);
	myObjectEntity->myOrientation.SetPos(orginalPos);
}

void DLLApp::ManualRotateObject()
{
	myAutoRotationSpeed = { 0, 0, 0 };
	CU::Matrix44f staticRotation;
	CU::Vector3f orginalPos(myObjectEntity->myOrientation.GetPos());
	myObjectEntity->myOrientation.SetPos(CU::Vector3f());
	staticRotation = CU::Matrix44f::CreateRotateAroundX(myManualRotationAngle.myX / (3.14f * 180));
	staticRotation = CU::Matrix44f::CreateRotateAroundY(myManualRotationAngle.myY / (3.14f * 180));
	staticRotation = CU::Matrix44f::CreateRotateAroundZ(myManualRotationAngle.myZ / (3.14f * 180));
	myObjectEntity->GetComponent<InputComponent>()->SetRotation(myObjectEntity->myOrientation * staticRotation);
	myObjectEntity->myOrientation.SetPos(orginalPos);
}

void DLLApp::CameraZoom(float aDeltaTime)
{
	if (HasMouseDeltaYMoved() == true)
	{
		myCamera->MoveForward(myInput.GetMouseDY() * myCameraZoomSpeed * aDeltaTime);
	}
	if (HasMouseDeltaXMoved() == true)
	{
		myCamera->MoveForward(myInput.GetMouseDX() * myCameraZoomSpeed * aDeltaTime);
	}
}

void DLLApp::CameraPan(float aDeltaTime)
{
	if (HasMouseDeltaXMoved() == true)
	{
		myCamera->MoveRight(myInput.GetMouseDX() * myCameraMovementSpeed * aDeltaTime);
	}
	if (HasMouseDeltaYMoved() == true)
	{
		myCamera->MoveUp(myInput.GetMouseDY() * myCameraMovementSpeed * aDeltaTime);
	}
}

void DLLApp::CameraRotation(float aDeltaTime)
{
	CU::Matrix44f rotationAroundObject;
	if (HasMouseDeltaYMoved() == true)
	{
		rotationAroundObject = myCamera->GetOrientation() * CU::Matrix44f::CreateRotateAroundX(myInput.GetMouseDY()
			* myCameraRotationSpeed * aDeltaTime);
		myCamera->SetOrientation(rotationAroundObject);
	}
	if (HasMouseDeltaXMoved() == true)
	{
		rotationAroundObject = myCamera->GetOrientation() * CU::Matrix44f::CreateRotateAroundY(myInput.GetMouseDX()
			* myCameraRotationSpeed * aDeltaTime);
		myCamera->SetOrientation(rotationAroundObject);
	}
}
void DLLApp::SetupCamera(Prism::SetupInfo& aWindowSetup)
{
	myCameraEntity = new Entity(eEntityType::PLAYER, *myScene, Prism::eOctreeType::DYNAMIC);
	myCameraEntity->AddComponent<InputComponent>()->Init(myInput);

	myCamera = new Prism::Camera(myCameraEntity->myOrientation);
	myCamera->OnResize(aWindowSetup.myScreenWidth, aWindowSetup.myScreenHeight);
	myScene->SetCamera(myCamera);
}

void DLLApp::SetupObject()
{
	myObjectEntity = new Entity(eEntityType::PROP, *myScene, Prism::eOctreeType::DYNAMIC);
	myObjectEntity->AddComponent<InputComponent>()->Init(myInput);
}

void DLLApp::SetupCubeMap()
{
	EngineInstance->GetEffectContainer()->SetCubeMap("Data/Resource/Texture/CubeMap/T_cubemap_test.dds");
	EngineInstance->SetClearColor(CU::Vector4f(0.3f, 0.3f, 0.3f, 1.f));
}

void DLLApp::SetupInput()
{
	myInput.Init(myPanelWindowHandler, GetModuleHandle(NULL),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

void DLLApp::SetParentWindow(Prism::SetupInfo &aWindowSetup)
{
	SetParent(myEngineWindowHandler, myPanelWindowHandler);
	SetWindowLong(myEngineWindowHandler, GWL_STYLE, WS_POPUP);
	SetWindowPos(myEngineWindowHandler, HWND_TOP, 0, 0,
		aWindowSetup.myScreenWidth, aWindowSetup.myScreenHeight, SWP_SHOWWINDOW);
}

void DLLApp::SetupLight()
{
	myDirectionalLight = new Prism::DirectionalLight();
	myDirectionalLight->SetDir(CU::Vector3f( 0.f, 0.f, -1.f ));
	myDirectionalLight->SetColor(CU::Vector4f(1.f, 1.f, 1.f, 1.f));

	myScene->AddLight(myDirectionalLight);
}
