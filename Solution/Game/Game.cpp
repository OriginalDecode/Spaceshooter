#include "stdafx.h"

#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "ColoursForBG.h"
#include <CommonHelper.h>
#include "Constants.h"
#include <Engine.h>
#include <FileWatcher.h>
#include <DebugFont.h>
#include "Game.h"
#include "GameStateMessage.h"
#include "InGameState.h"
#include <InputWrapper.h>
#include "Level.h"
#include "LevelFactory.h"
#include "MenuState.h"
#include "PostMaster.h"
#include "ResizeMessage.h"
#include <SystemMonitor.h>
#include <TimerManager.h>
#include <VTuneApi.h>
#include <Vector.h>
#include <XMLReader.h>

Game::Game()
	: myLockMouse(true)
#ifdef RELEASE_BUILD
	, myShowSystemInfo(false)
#else
	, myShowSystemInfo(true)
#endif
{
	PostMaster::Create();
	Prism::Audio::AudioInterface::CreateInstance();
	myInputWrapper = new CU::InputWrapper();
	Prism::Engine::GetInstance()->SetShowDebugText(myShowSystemInfo);

	
}

Game::~Game()
{
	delete myInputWrapper;
	PostMaster::GetInstance()->UnSubscribe(eMessageType::GAME_STATE, this);

	Prism::Audio::AudioInterface::Destroy();
	PostMaster::Destroy();
}

bool Game::Init(HWND& aHwnd)
{
	bool startInMenu = false;

	XMLReader reader;
	reader.OpenDocument("Data/Setting/SET_options.xml");
	reader.ReadAttribute(reader.FindFirstChild("startInMenu"), "bool", startInMenu);
#ifdef _DEBUG
	startInMenu = false;
#endif
	reader.ReadAttribute(reader.FindFirstChild("showMessages"), "bool", myShowMessages);
	reader.CloseDocument();
	PostMaster::GetInstance()->Subscribe(eMessageType::GAME_STATE, this);

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Resource/Sound/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Resource/Sound/SpaceShooterBank.bnk");

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	if (startInMenu == false)
	{
		PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::LOAD_GAME, 1));
	}
	else
	{
		myCurrentMenu = new MenuState("Data/Menu/MN_main_menu.xml", myInputWrapper);
		myStateStack.PushMainGameState(myCurrentMenu);
		myLockMouse = false;
	}

	Prism::Engine::GetInstance()->SetClearColor({ MAGENTA });

	myWindowSize.x = Prism::Engine::GetInstance()->GetWindowSize().x;
	myWindowSize.y = Prism::Engine::GetInstance()->GetWindowSize().y;


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
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();

	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	if (deltaTime > 1.0f / 10.0f)
	{
		deltaTime = 1.0f / 10.0f;
	}

	if (myInputWrapper->KeyUp(DIK_O) == true)
	{
		myLockMouse = !myLockMouse;
		ShowCursor(!myLockMouse);
	}

	if (myLockMouse == true)
	{
		SetCursorPos(myWindowSize.x / 2, myWindowSize.y / 2);
	}

	if (myStateStack.UpdateCurrentState(deltaTime) == false)
	{
		return false;
	}

	if (myInputWrapper->KeyDown(DIK_F8))
	{
		myShowSystemInfo = !myShowSystemInfo;
		Prism::Engine::GetInstance()->SetShowDebugText(myShowSystemInfo);
	}
	if (myInputWrapper->KeyDown(DIK_F9))
	{
		Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	}


	myStateStack.RenderCurrentState();

	if (myShowSystemInfo == true)
	{
		int fps = int(1.f / deltaTime);
		float frameTime = deltaTime * 1000.f;
		int memory = Prism::SystemMonitor::GetMemoryUsageMB();
		float cpuUsage = Prism::SystemMonitor::GetCPUUsage();

		Prism::Engine::GetInstance()->PrintText(CU::Concatenate("FPS: %d", fps), { 1000.f, -20.f }, Prism::eTextType::DEBUG_TEXT);
		Prism::Engine::GetInstance()->PrintText(CU::Concatenate("FrameTime: %f", frameTime), { 1000.f, -50.f }, Prism::eTextType::DEBUG_TEXT);
		Prism::Engine::GetInstance()->PrintText(CU::Concatenate("Mem: %d (MB)", memory), { 1000.f, -80.f }, Prism::eTextType::DEBUG_TEXT);
		Prism::Engine::GetInstance()->PrintText(CU::Concatenate("CPU: %f", cpuUsage), { 1000.f, -110.f }, Prism::eTextType::DEBUG_TEXT);
	}

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
	Prism::Engine::GetInstance()->OnResize(aWidth, aHeight);
	myWindowSize.x = aWidth;
	myWindowSize.y = aHeight;
	myStateStack.OnResizeCurrentState(aWidth, aHeight);
	PostMaster::GetInstance()->SendMessage(ResizeMessage(aWidth, aHeight));
}

void Game::ReceiveMessage(const GameStateMessage& aMessage)
{
	switch (aMessage.GetGameState())
	{
	case eGameState::LOAD_GAME:
		myGame = new InGameState(myInputWrapper, myShowMessages);
		myStateStack.PushMainGameState(myGame);
		myGame->SetLevel(aMessage.GetID());
		break;
	case eGameState::LOAD_MENU:
		myCurrentMenu = new MenuState(aMessage.GetFilePath(), myInputWrapper);
		myStateStack.PushMainGameState(myCurrentMenu);
		break;
	case eGameState::MOUSE_LOCK:
		myLockMouse = aMessage.GetMouseLocked();
		break;
	}
}