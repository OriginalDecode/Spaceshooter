#pragma warning(disable : 4005)

#include "DLLExport.h"
#include <Engine.h>
#include <Windows.h>
#include <SetupInfo.h>
#include <DL_Debug.h>
#include <EngineEnums.h>
#include <Entity.h>
#include <FileWatcher.h>
#include <InputComponent.h>
#include <GraphicsComponent.h>
#include <Game.h>
#include <TimerManager.h>
#include <Scene.h>
#include <Instance.h>
#include <Model.h>
#include <ModelLoader.h>
#include <ModelProxy.h>
#include <EffectContainer.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <TimerManager.h>
#include <InputWrapper.h>
#include <PostMaster.h>
#include <AudioInterface.h>

#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND locEngineWindowHandler;
HWND locPanelWindowHandler;
Prism::SetupInfo locWindowSetup;
Prism::Scene* locScene;
Prism::Model* locModel;
Prism::ModelProxy* locModelProxy;
Prism::Instance* locInstance;
Prism::Camera* locCamera;
Prism::DirectionalLight* locDirectionLight;
CU::Matrix44f locPlayerPos;
CU::Matrix44f locStaticRotation;
CU::InputWrapper locInput;
float locDeltaTime = 0;

CU::Vector3f locAutoRotationSpeed;

std::string locCurrentModelFile;
std::string locCurrentEffectFile;

Entity* locObjectEntity;
Entity* locCameraEntity;

void StartEngine(int* aHwnd)
{
	DL_Debug::Debug::Create();
	CU::TimerManager::Create();

	PostMaster::Create();
	Prism::Audio::AudioInterface::CreateInstance();

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Resource/Sound/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Resource/Sound/SpaceShooterBank.bnk");

	locScene = new Prism::Scene();
	locPanelWindowHandler = (HWND)aHwnd;
	DL_DEBUG("WindowHandler Set");
	Prism::Engine::Create(locEngineWindowHandler, WndProc, locWindowSetup);
	DL_DEBUG("Engine Created");
	locDirectionLight = new Prism::DirectionalLight();
	locDirectionLight->SetDir({ 0.f, 0.5f, -1.f });
	locDirectionLight->SetColor({ 0.7f, 0.7f, 0.7f, 1.f });
	DL_DEBUG("Direction light created");
	locScene->AddLight(locDirectionLight);
	DL_DEBUG("Direction light Set");
	locInput.Init(locPanelWindowHandler, GetModuleHandle(NULL),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	DL_DEBUG("Input Set");
	SetParent(locEngineWindowHandler, locPanelWindowHandler);
	SetWindowLong(locEngineWindowHandler, GWL_STYLE, WS_POPUP);
	SetWindowPos(locEngineWindowHandler, HWND_TOP, 0, 0, locWindowSetup.myScreenWidth,
		locWindowSetup.myScreenHeight, SWP_SHOWWINDOW);
	DL_DEBUG("Window Resize to Panel");
	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap("Data/Resource/Texture/CubeMap/T_cubemap_test.dds");
	Prism::Engine::GetInstance()->SetClearColor({ 0.3f, 0.3f, 0.3f, 1.f });
	DL_DEBUG("Cubemap and clear color Set");
	std::stringstream ss;
	ss << "Scene address: " << locScene << "\nWindow Handler address: " << &locPanelWindowHandler << "\nEngine Handler adress: " << &locEngineWindowHandler;
	DL_DEBUG(ss.str().c_str());
	DL_DEBUG(ss.str().c_str());
	locObjectEntity = new Entity(eEntityType::PROP, *locScene, Prism::eOctreeType::DYNAMIC);
	DL_DEBUG("Entity Created");
	locObjectEntity->AddComponent<InputComponent>()->Init(locInput);
	DL_DEBUG("Entity Input Set");
	locCameraEntity = new Entity(eEntityType::PLAYER, *locScene, Prism::eOctreeType::DYNAMIC);
	locCamera = new Prism::Camera(locCameraEntity->myOrientation);
	DL_DEBUG("Camera Created");
	locScene->SetCamera(locCamera);
	DL_DEBUG("Camera Set");
	locCamera->OnResize(locWindowSetup.myScreenWidth, locWindowSetup.myScreenHeight);
	DL_DEBUG("Camera Resize");
	locCameraEntity->AddComponent<InputComponent>()->Init(locInput);
	DL_DEBUG("Camera Input Set");
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
	locScene->Render();
}

void RotateObjectAtY(float aSpeed, float deltaTime)
{
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locObjectEntity->GetComponent<InputComponent>()->RotateY(aSpeed * deltaTime);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void RotateObjectAtX(float aSpeed, float deltaTime)
{
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locObjectEntity->GetComponent<InputComponent>()->RotateX(aSpeed * deltaTime);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void RotateObjectAtZ(float aSpeed, float deltaTime)
{
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locObjectEntity->GetComponent<InputComponent>()->RotateZ(aSpeed * deltaTime);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void RotateObjectAtY(float aSpeed)
{
	locAutoRotationSpeed.myY = aSpeed;
}

void RotateObjectAtX(float aSpeed)
{
	locAutoRotationSpeed.myX = aSpeed;
}

void RotateObjectAtZ(float aSpeed)
{
	locAutoRotationSpeed.myZ = aSpeed;
}

void SetRotateObjectAtY(float aAngle)
{
	locAutoRotationSpeed = { 0, 0, 0 };
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locStaticRotation = CU::Matrix44f::CreateRotateAroundY(aAngle / (3.14f * 180));
	locObjectEntity->GetComponent<InputComponent>()->SetRotation(locObjectEntity->myOrientation * locStaticRotation);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void SetRotateObjectAtX(float aAngle)
{
	locAutoRotationSpeed = { 0, 0, 0 };
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locStaticRotation = CU::Matrix44f::CreateRotateAroundX(aAngle / (3.14f * 180));
	locObjectEntity->GetComponent<InputComponent>()->SetRotation(locObjectEntity->myOrientation * locStaticRotation);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void SetRotateObjectAtZ(float aAngle)
{
	locAutoRotationSpeed = { 0, 0, 0 };
	CU::Vector3f orginalPos(locObjectEntity->myOrientation.GetPos());
	locObjectEntity->myOrientation.SetPos(CU::Vector3f());
	locStaticRotation = CU::Matrix44f::CreateRotateAroundZ(aAngle / (3.14f * 180));
	locObjectEntity->GetComponent<InputComponent>()->SetRotation(locObjectEntity->myOrientation * locStaticRotation);
	locObjectEntity->myOrientation.SetPos(orginalPos);
}

void Update()
{
	CU::TimerManager::GetInstance()->Update();
	float locDeltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	locInput.Update();
	if (locInput.KeyIsPressed(DIK_ADD) || locInput.GetMouseDZ() < 0)
	{
		locCamera->MoveForward(10.f * locDeltaTime);
	}
	if (locInput.KeyIsPressed(DIK_SUBTRACT) || locInput.GetMouseDZ() > 0)
	{
		locCamera->MoveForward(-10.f * locDeltaTime);
	}

	RotateObjectAtX(locAutoRotationSpeed.myX, locDeltaTime);
	RotateObjectAtY(locAutoRotationSpeed.myY, locDeltaTime);
	RotateObjectAtZ(locAutoRotationSpeed.myZ, locDeltaTime);
}

void UpdateFilewatcher() 
{
	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
}

void ReloadModel() 
{
	DL_DEBUG("Reload start!");

	LoadModel(locCurrentModelFile.c_str(), locCurrentEffectFile.c_str());
	std::stringstream watchMsg;
	watchMsg << "[FileWatcher]: Reload " << locCurrentModelFile << " and " << locCurrentEffectFile;
	DL_DEBUG(watchMsg.str().c_str());
}

void WatchCurrentFiles(const char* aModelFile, const char* aEffectFile)
{
	Prism::Engine::GetInstance()->GetFileWatcher()->Clear();
	Prism::Engine::GetInstance()->GetFileWatcher()->WatchFile(aModelFile, ReloadModel);

	locCurrentModelFile = aModelFile;
	locCurrentEffectFile = aEffectFile;

	std::stringstream watchMsg;
	watchMsg <<"[FileWatcher]: Watch " << aModelFile << " and " << aEffectFile;
	DL_DEBUG(watchMsg.str().c_str());
}

void LoadModel(const char* aModelFile, const char* aEffectFile)
{

	if (aEffectFile == "")
	{
		aEffectFile = "Data/Resource/Shader/S_effect_basic.fx";
	}
	
	WatchCurrentFiles(aModelFile, aEffectFile);
	CU::Matrix44f currentOrientation = locObjectEntity->myOrientation;
	delete locObjectEntity;

	locObjectEntity = new Entity(eEntityType::PROP, *locScene, Prism::eOctreeType::DYNAMIC);
	locObjectEntity->AddComponent<InputComponent>()->Init(locInput);

	locObjectEntity->AddComponent<GraphicsComponent>()->InitDLL(aModelFile, aEffectFile);
	locObjectEntity->myOrientation = currentOrientation;
	GraphicsComponent* gfxComponent = locObjectEntity->GetComponent<GraphicsComponent>();
	gfxComponent->GetInstance()->SetEffect(aEffectFile);

	locInstance = gfxComponent->GetInstance();

	locScene->AddInstance(locInstance);
}

void SetEffect(const char* aEffectFile)
{
	if (locInstance != nullptr)
	{
		std::stringstream ss; ss << "Shader: " << aEffectFile;
		DL_DEBUG(ss.str().c_str());
		/*locInstance->SetEffect(aEffectFile);*/
	}
}

void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel)
{
	Prism::Engine::GetInstance()->SetClearColor({ aRChannel, aGChannel, aBChannel, aAChannel });
	std::stringstream ss;
	ss << "R: " << aRChannel << ", G: " << aGChannel << ", B: " << aBChannel << ", A: " << aAChannel;
	DL_DEBUG(ss.str().c_str());
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

float GetDirectionaLightXRotation()
{
	return locDirectionLight->GetCurrentDir().myX;
}

float GetDirectionaLightYRotation()
{
	return locDirectionLight->GetCurrentDir().myY;
}

float GetDirectionaLightZRotation()
{
	return locDirectionLight->GetCurrentDir().myZ;
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