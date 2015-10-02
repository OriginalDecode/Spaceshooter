#include "stdafx.h"
#include "LevelSelectState.h"
#include <Model2D.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "StateStackProxy.h"
#include "PostMaster.h"
#include "GameStateMessage.h"

LevelSelectState::LevelSelectState(CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
}

LevelSelectState::~LevelSelectState()
{
}

void LevelSelectState::InitState(StateStackProxy* aStateStackProxy)
{
	myStateStack = aStateStackProxy;
	myBackground = new Prism::Model2D;
	myBackground->Init("Data/resources/texture/skyTest.dds", { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) });
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
}

void LevelSelectState::EndState()
{
	delete myBackground;
	delete myCamera;
}

const eStateStatus LevelSelectState::Update()
{
	if (myInputWrapper->KeyDown(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopSubState;
	}

	if (myInputWrapper->KeyDown(DIK_1) == true)
	{
		PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::INGAME_STATE));
	}

	Render();

	return eKeepState;
}

void LevelSelectState::Render()
{
	myBackground->Render(*myCamera, 0.f, -100.f);
}

void LevelSelectState::ResumeState()
{

}

void LevelSelectState::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}