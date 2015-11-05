#include "stdafx.h"

#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionManager.h"
#include "ColoursForBG.h"
#include "Constants.h"
#include <Engine.h>
#include "Entity.h"
#include <FileWatcher.h>
#include <DebugFont.h>
#include "GameStateMessage.h"
#include "InGameState.h"
#include "InGameMenuState.h"
#include <InputWrapper.h>
#include "Level.h"
#include "LevelFactory.h"
#include "MessageState.h"
#include <ModelLoader.h>
#include "PostMaster.h"
#include <TimerManager.h>
#include <VTuneApi.h>
#include <Vector.h>

InGameState::InGameState(CU::InputWrapper* anInputWrapper, const bool& aShowMessages)
	: myShowMessages(aShowMessages)
	, myPlayer(nullptr)
{
	myInputWrapper = anInputWrapper;
}

InGameState::~InGameState()
{
	delete myLevelFactory;
	delete myLevel;
	delete myPlayer;
	myLevelFactory = nullptr;
	myLevel = nullptr;
	myPlayer = nullptr;
	PostMaster::GetInstance()->UnSubscribe(eMessageType::GAME_STATE, this);
}

void InGameState::InitState(StateStackProxy* aStateStackProxy)
{
	myLevelFactory = new LevelFactory("Data/Level/LI_list_level.xml", myInputWrapper, *myPlayer);
	myIsLetThrough = false;
	myIsComplete = false;
	myStateStack = aStateStackProxy;
	myStateStatus = eStateStatus::eKeepState;
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y); 
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));
	PostMaster::GetInstance()->Subscribe(eMessageType::GAME_STATE, this);
}

void InGameState::EndState()
{
}

const eStateStatus InGameState::Update(const float& aDeltaTime)
{
	myPlayer = myLevel->GetPlayer();

	if (myInputWrapper->KeyDown(DIK_ESCAPE) || myIsComplete == true)
	{
		myStateStack->PushSubGameState(new InGameMenuState("Data/Menu/MN_ingame_menu.xml", myInputWrapper));

	}

	if (myLevel->LogicUpdate(aDeltaTime) == true)
	{
		GameStateMessage* newEvent = new GameStateMessage(eGameState::RELOAD_LEVEL);
		ShowMessage("Data/Resource/Texture/Menu/MainMenu/T_background_default.dds", { 600, 400 }, "Game over! Press [space] to continue.", newEvent);
		return eStateStatus::eKeepState;
	}

	return eStateStatus::eKeepState;
}

void InGameState::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	myLevel->Render();

	VTUNE_EVENT_END();
}

void InGameState::ResumeState()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));
}

void InGameState::OnResize(int aWidth, int aHeight)
{
	if (myLevel != nullptr)
	{
		myLevel->OnResize(aWidth, aHeight);
	}
}

void InGameState::ReceiveMessage(const GameStateMessage& aMessage)
{
	switch (aMessage.GetGameState())
	{
	case eGameState::RELOAD_LEVEL:
		myLevel = myLevelFactory->LoadCurrentLevel();
		break;

	case eGameState::COMPLETE_LEVEL:
		if (myLevelFactory->IsLastLevel() == false)
		{
			CompleteLevel();
		}
		else
		{
			CompleteGame();
		}
		break;

	case eGameState::LOAD_NEXT_LEVEL:
		myLevel = myLevelFactory->LoadNextLevel();
		break;
	}
}

void InGameState::SetLevel(int aLevelID)
{
	myLevel = myLevelFactory->LoadLevel(aLevelID);
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

void InGameState::LoadLevelSettings()
{
	myLevelFactory->ReadLevelSettings();
}

void InGameState::LoadPlayerSettings()
{
	myLevelFactory->LoadPlayer();
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