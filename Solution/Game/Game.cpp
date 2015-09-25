#include "stdafx.h"

#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "ColoursForBG.h"
#include "Constants.h"
#include <DebugDataDisplay.h>
#include <FileWatcher.h>
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include <InputWrapper.h>
#include "Level.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include "PostMaster.h"
#include <Vector.h>

Game::Game()
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
	myStateStack.OnResizeCurrentState(aWidth, aHeight);
}