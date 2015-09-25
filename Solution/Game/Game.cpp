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
	myBulletManager = new BulletManager;

	//ShowCursor(false);
}

Game::~Game()
{
	
	delete myInputWrapper;
	delete myBulletManager;

	Prism::Audio::AudioInterface::Destroy();
	PostMaster::Destroy();
}

bool Game::Init(HWND& aHwnd)
{

	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myLevel = new Level("Data/script/level1.xml", myInputWrapper, myBulletManager);

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
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	if (deltaTime > 1.0f / 10.0f)
	{
		deltaTime = 1.0f / 10.0f;
	}

	if (myInputWrapper->KeyDown(DIK_F9))
	{
		myLevel->SetShowLightCube(!myLevel->GetShowLightCube());
	}
	else if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myLevel->SetRenderStuff(!myLevel->GetRenderStuff());
	}
	if (myInputWrapper->KeyDown(DIK_P))
	{
		Prism::Engine::GetInstance()->ToggleWireframe();
	}

	LogicUpdate(deltaTime);

	myBulletManager->Update(deltaTime);

	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();

	END_TIME_BLOCK("Game::Update");


	Render();


	Prism::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	Prism::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(deltaTime);
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
	myLevel->OnResize(aWidth, aHeight);
}

void Game::LogicUpdate(const float aDeltaTime)
{
	myLevel->LogicUpdate(aDeltaTime);
}

void Game::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	BEGIN_TIME_BLOCK("Game::Render");

	myLevel->Render();


	END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay()->Render();

	VTUNE_EVENT_END();
}

