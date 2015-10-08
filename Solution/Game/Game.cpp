#include "stdafx.h"

#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "ColoursForBG.h"
#include "Constants.h"
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <FileWatcher.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include "InGameState.h"
#include <InputWrapper.h>
#include "Level.h"
#include "LevelFactory.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include "PostMaster.h"
#include <Vector.h>
#include "GameStateMessage.h"
#include "MenuState.h"
#include <XMLReader.h>

Game::Game()
	: myLockMouse(true)
{
	PostMaster::Create();
	Prism::Audio::AudioInterface::CreateInstance();
	myInputWrapper = new CU::InputWrapper();
}

Game::~Game()
{
	delete myInputWrapper;
	delete myLevelFactory;

	Prism::Audio::AudioInterface::Destroy();
	PostMaster::Destroy();
}

bool Game::Init(HWND& aHwnd)
{
	bool startInMenu = false;

	XMLReader reader;
	reader.OpenDocument("Data/script/options.xml");
	reader.ReadAttribute(reader.FindFirstChild("startInMenu"), "bool", startInMenu);
	reader.ReadAttribute(reader.FindFirstChild("canWinGame"), "bool", myCanWinGame);

	PostMaster::GetInstance()->Subscribe(eMessageType::GAME_STATE, this);

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Audio/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Audio/SpaceShooterBank.bnk");

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myLevelFactory = new LevelFactory("Data/level/levelList.xml", myInputWrapper, myCanWinGame);

	if (startInMenu == false)
	{
		myCurrentMenu = new MenuState("Data/script/MainMenu.xml", myInputWrapper);
		myStateStack.PushMainGameState(myCurrentMenu);
		myGame = new InGameState(myInputWrapper);
		myGame->SetLevel(myLevelFactory->LoadLevel(1));
		myStateStack.PushMainGameState(myGame);
	}
	else
	{
		myCurrentMenu = new MenuState("Data/script/MainMenu.xml", myInputWrapper);
		myStateStack.PushMainGameState(myCurrentMenu);
		myLockMouse = false;
	}

	Prism::Engine::GetInstance()->SetClearColor({ MAGENTA });


	ADD_FUNCTION_TO_RADIAL_MENU("Toggle FPS", Prism::DebugDataDisplay::ToggleFrameTime, Prism::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Graph", Prism::DebugDataDisplay::ToggleFunctionTimers, Prism::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Mem", Prism::DebugDataDisplay::ToggleMemoryUsage, Prism::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle CPU", Prism::DebugDataDisplay::ToggleCPUUsage, Prism::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Wireframe", Prism::Engine::ToggleWireframe, Prism::Engine::GetInstance());

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
	BEGIN_TIME_BLOCK("Game::Update");
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();

	if (myInputWrapper->KeyUp(DIK_O) == true)
	{
		myLockMouse = !myLockMouse;
		ShowCursor(!myLockMouse);
	}

	if (myLockMouse == true)
	{
		SetCursorPos(myWindowSize.x / 2, myWindowSize.y / 2);
	}

	if (myStateStack.UpdateCurrentState() == false)
	{
		return false;
	}
	myStateStack.RenderCurrentState();


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
	myWindowSize.x = aWidth;
	myWindowSize.y = aHeight;
	myStateStack.OnResizeCurrentState(aWidth, aHeight);
}

void Game::ReceiveMessage(const GameStateMessage& aMessage)
{
	switch (aMessage.GetGameState())
	{
	case eGameState::LOAD_GAME:
		myGame = new InGameState(myInputWrapper);
		myGame->SetLevel(myLevelFactory->LoadLevel(aMessage.GetID()));
		myStateStack.PushMainGameState(myGame);
		break;

	case eGameState::CHANGE_LEVEL:
		myGame->SetLevel(myLevelFactory->LoadLevel(aMessage.GetID()));
		break;

	case eGameState::RELOAD_LEVEL:
		myGame->SetLevel(myLevelFactory->ReloadLevel());
		break;

	case eGameState::LOAD_MENU:
		myCurrentMenu = new MenuState(aMessage.GetFilePath(), myInputWrapper);
		myStateStack.PushMainGameState(myCurrentMenu);
		break;

	case eGameState::COMPLETE_LEVEL:
		myGame->SetLevel(myLevelFactory->LoadNextLevel());
		break;

	case eGameState::MOUSE_LOCK:
		myLockMouse = aMessage.GetMouseLocked();
		break;

	case eGameState::COMPLETE_GAME:
		myGame->CompleteGame();
		break;
	}
}