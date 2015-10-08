#include "stdafx.h"

#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionManager.h"
#include "ColoursForBG.h"
#include "Constants.h"
#include <DebugDataDisplay.h>
#include <FileWatcher.h>
#include <Font.h>
#include "GameStateMessage.h"
#include <GeometryGenerator.h>
#include "InGameState.h"
#include <InputWrapper.h>
#include "Level.h"
#include "MessageState.h"
#include "PostMaster.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include <Vector.h>

InGameState::InGameState(CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
}

InGameState::~InGameState()
{
	delete myLevel;
}

void InGameState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = false;
	myIsComplete = false;
	myStateStack = aStateStackProxy;
	myStateStatus = eStateStatus::eKeepState;
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y); // very needed here, don't remove
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));
}

void InGameState::EndState()
{
}

const eStateStatus InGameState::Update()
{
	BEGIN_TIME_BLOCK("InGameState::Update");

	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	if (deltaTime > 1.0f / 10.0f)
	{
		deltaTime = 1.0f / 10.0f;
	}

	else if (myInputWrapper->KeyDown(DIK_ESCAPE) || myIsComplete == true)
	{
		return eStateStatus::ePopMainState;
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myLevel->SetRenderStuff(!myLevel->GetRenderStuff());
	}

	if (myLevel->LogicUpdate(deltaTime) == true)
	{
		myMessageScreen = new MessageState("Data/resources/texture/menu/MainMenu/background.dds", { 600, 400 }, myInputWrapper);
		myMessageScreen->SetText("Game over! Press [space] to continue.");
		GameStateMessage* event = new GameStateMessage(eGameState::RELOAD_LEVEL);
		myMessageScreen->SetEvent(event);
		myStateStack->PushSubGameState(myMessageScreen);
		return eStateStatus::eKeepState;
	}

	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();

	END_TIME_BLOCK("InGameState::Update");


	Prism::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	Prism::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(deltaTime);

	return eStateStatus::eKeepState;
}

void InGameState::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	BEGIN_TIME_BLOCK("Game::Render");

	myLevel->Render();


	END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay()->Render();

	VTUNE_EVENT_END();
}

void InGameState::ResumeState()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));
}

void InGameState::OnResize(int aWidth, int aHeight)
{
	myLevel->OnResize(aWidth, aHeight);
}

void InGameState::CompleteGame()
{
	myMessageScreen = new MessageState("Data/resources/texture/menu/MainMenu/background.dds", { 600, 400 }, myInputWrapper);
	myMessageScreen->SetText("Game won! Press [space] to continue.");
	myStateStack->PushSubGameState(myMessageScreen);
	myIsComplete = true;
}