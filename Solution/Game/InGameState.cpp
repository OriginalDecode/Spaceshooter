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

InGameState::InGameState(CU::InputWrapper* anInputWrapper, const bool& aShowMessages)
	: myShowMessages(aShowMessages)
{
	myInputWrapper = anInputWrapper;
}

InGameState::~InGameState()
{
	delete myLevel;
	myLevel = nullptr;
}

void InGameState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = false;
	myIsComplete = false;
	myStateStack = aStateStackProxy;
	myStateStatus = eStateStatus::eKeepState;
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y); 
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
		GameStateMessage* newEvent = new GameStateMessage(eGameState::RELOAD_LEVEL);
		ShowMessage("Data/Resource/Texture/Menu/MainMenu/T_background_default.dds", { 600, 400 }, "Game over! Press [space] to continue.", newEvent);
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

void InGameState::CompleteLevel()
{
	GameStateMessage* newEvent = new GameStateMessage(eGameState::LOAD_NEXT_LEVEL);
	ShowMessage("Data/Resource/Texture/Menu/MainMenu/T_background_default.dds", { 600, 400 }, "Level complete! Press [space] to continue.", newEvent);
}

void InGameState::CompleteGame()
{
	ShowMessage("Data/Resource/Texture/Menu/MainMenu/T_background_default.dds", { 600, 400 }, "Game won! Press [space] to continue.");
	myIsComplete = true;
}

void InGameState::ShowMessage(const std::string& aBackgroundPath, 
	const CU::Vector2<float>& aSize, std::string aText, GameStateMessage* aMessage)
{
	if (myShowMessages == true)
	{
		myMessageScreen = new MessageState(aBackgroundPath, aSize, myInputWrapper);
		myMessageScreen->SetText(aText);
		myMessageScreen->SetEvent(aMessage);
		myStateStack->PushSubGameState(myMessageScreen);
	}
	else if (aMessage != nullptr)
	{
		PostMaster::GetInstance()->SendMessage(*aMessage);
	}
}