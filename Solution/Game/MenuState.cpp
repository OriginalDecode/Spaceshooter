#include "stdafx.h"
#include "MenuState.h"
#include <Sprite.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "StateStackProxy.h"
#include "PostMaster.h"
#include "GameStateMessage.h"
#include "Menu.h"
#include "Button.h"

MenuState::MenuState(const std::string& aXMLPath, CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
	
	myMenu = new Menu(aXMLPath);
}

MenuState::~MenuState()
{
}

void MenuState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = false;
	myStateStack = aStateStackProxy;
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, false));
}

void MenuState::EndState()
{
	delete myMenu;
	delete myCamera;
	myMenu = nullptr;
	myCamera = nullptr;
}

const eStateStatus MenuState::Update(const float&)
{
	if (myInputWrapper->KeyDown(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}

	myMenu->Update(myInputWrapper);

	return eKeepState;
}

void MenuState::Render()
{
	myMenu->Render(myInputWrapper);
}

void MenuState::ResumeState()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, false));
}

void MenuState::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
	myMenu->OnResize(aWidth, aHeight);
}