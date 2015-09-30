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
#include "MainMenuState.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include "PostMaster.h"
#include <Vector.h>

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

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myStateStack.SetInputWrapper(myInputWrapper);
	myGame = new InGameState();
	myStateStack.PushMainGameState(myGame);

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
	}

	if (myLockMouse == true)
	{
		SetCursorPos(myWindowSize.x / 2, myWindowSize.y / 2);
	}

	if (myStateStack.UpdateCurrentState() == false)
	{
		return false;
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
	myWindowSize.x = aWidth;
	myWindowSize.y = aHeight;
	myStateStack.OnResizeCurrentState(aWidth, aHeight);
}