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

	Prism::Audio::AudioInterface::Destroy();
	PostMaster::Destroy();
}

bool Game::Init(HWND& aHwnd)
{
	bool startInMeu = false;
	XMLReader reader;
	reader.OpenDocument("Data/script/options.xml");
	reader.ReadAttribute(reader.FindFirstChild("startInMenu"), "bool", startInMeu);

	PostMaster::GetInstance()->Subscribe(eMessageType::GAME_STATE, this);

	Prism::Audio::AudioInterface::GetInstance()->Init("Data/Audio/Init.bnk");
	Prism::Audio::AudioInterface::GetInstance()->LoadBank("Data/Audio/SpaceShooterBank.bnk");

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myMainMenu = new MenuState("Data/script/MainMenu.xml", myInputWrapper);
	myLevelSelect = new MenuState("Data/script/levelSelect.xml", myInputWrapper);
	myGame = new InGameState(myInputWrapper, "Data/script/level1.xml", false);

	if (startInMeu == false)
	{
		myStateStack.PushMainGameState(myGame);
	}
	else
	{
		myStateStack.PushMainGameState(myMainMenu);
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
	case eGameState::INGAME_STATE:
		myLockMouse = true;
		myGame = new InGameState(myInputWrapper, aMessage.GetFilePath(), aMessage.myUseXML);
		myStateStack.PushMainGameState(myGame);
		break;
	case eGameState::MAIN_MENU_STATE:
		myLockMouse = false;
		myStateStack.PushMainGameState(myMainMenu);
		break;
	case eGameState::LEVEL_SELECT_STATE:
		myLockMouse = false;
		myLevelSelect = new MenuState("Data/script/levelSelect.xml", myInputWrapper);
		myStateStack.PushMainGameState(myLevelSelect);
		break;
	}
}