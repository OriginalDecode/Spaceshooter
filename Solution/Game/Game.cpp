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
#include "GameStateMessage.h"
#include "InGameState.h"
#include <InputWrapper.h>
#include "Level.h"
#include "LevelFactory.h"
#include "MenuState.h"
#include "PostMaster.h"
#include "ResizeMessage.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include <Vector.h>
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

	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	Prism::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	Prism::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(deltaTime);


	myStateStack.RenderCurrentState();
	Prism::Engine::GetInstance()->GetDebugDisplay()->Render();

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