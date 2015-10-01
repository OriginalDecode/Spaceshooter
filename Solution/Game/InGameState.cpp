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
#include <GeometryGenerator.h>
#include "InGameState.h"
#include <InputWrapper.h>
#include "Level.h"
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
	delete myBulletManager;
	delete myCollisionManager;
}

void InGameState::InitState()
{
	myStateStatus = eStateStatus::eKeepState;
	myBulletManager = new BulletManager;
	myCollisionManager = new CollisionManager();
	myBulletManager->SetCollisionManager(myCollisionManager);
	myLevel = new Level("Data/script/level1.xml", myInputWrapper, *myBulletManager, *myCollisionManager, false);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y); // very needed here, don't remove
}

void InGameState::EndState()
{
}

const eStateStatus InGameState::Update()
{
	BEGIN_TIME_BLOCK("InGameState::Update");
	
	myCollisionManager->CleanUp();

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
		return eStateStatus::ePopMainState;
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

	END_TIME_BLOCK("InGameState::Update");


	Render();


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

}

void InGameState::LogicUpdate(const float aDeltaTime)
{
	myLevel->LogicUpdate(aDeltaTime);
}

void InGameState::OnResize(int aWidth, int aHeight)
{
	myLevel->OnResize(aWidth, aHeight);
}