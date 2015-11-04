#include "stdafx.h"
#include "MenuState.h"
#include <Sprite.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "SplashState.h"
#include "StateStackProxy.h"
#include <TimerManager.h>
#include "PostMaster.h"
#include "GameStateMessage.h"
#include "Menu.h"
#include "Button.h"

MenuState::MenuState(const std::string& aXMLPath, CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
	
	myMenu = new Menu(aXMLPath);

	CU::Vector2<float> windowSize = CU::Vector2<float>(Prism::Engine::GetInstance()->GetWindowSize().x,
		Prism::Engine::GetInstance()->GetWindowSize().y);

	CU::Vector2<float> overlaySize(4096.f, 4096.f);
	myBlackOverlay = new Prism::Sprite("Data/Resource/Texture/Menu/Splash/T_background_default.dds", windowSize, windowSize / 2.f);
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

	if (myMenu->GetMainMenu() == true)
	{
		myStateStack->PushSubGameState(new SplashState("Data/Resource/Texture/Menu/Splash/T_logo_other.dds", myInputWrapper));
		myStateStack->PushSubGameState(new SplashState("Data/Resource/Texture/Menu/Splash/T_logo_our.dds", myInputWrapper));
	}
	myCurrentTime = 0;
	myFadeInTime = 0.5f;
	myOverlayAlpha = 1.f;
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
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	deltaTime = fminf(1.f / 30.f, deltaTime);

	myCurrentTime += deltaTime;
	myOverlayAlpha = fmaxf(1.f - myCurrentTime / myFadeInTime, 0);

	myMenu->Update(myInputWrapper);

	return eKeepState;
}

void MenuState::Render()
{
	myMenu->Render(myInputWrapper);
	CU::Vector2<float> windowSize = CU::Vector2<float>(Prism::Engine::GetInstance()->GetWindowSize().x,
		-Prism::Engine::GetInstance()->GetWindowSize().y);
	myBlackOverlay->Render(windowSize / 2.f, { 1.f, 1.f }, { 1.f, 1.f, 1.f, myOverlayAlpha});
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