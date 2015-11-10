#include "stdafx.h"

#include "Button.h"
#include <Camera.h>
#include "FadeMessage.h"
#include "GameStateMessage.h"
#include <InputWrapper.h>
#include "Menu.h"
#include "MenuState.h"
#include <Sprite.h>
#include "SplashState.h"
#include "StateStackProxy.h"
#include <TimerManager.h>
#include "PostMaster.h"

MenuState::MenuState(const std::string& aXMLPath, CU::InputWrapper* anInputWrapper, bool aShowVictoryScreen)
	: myHasFadeIn(aShowVictoryScreen)
	, myShowVictoryScreen(aShowVictoryScreen)
{
	myInputWrapper = anInputWrapper;
	
	myMenu = new Menu(aXMLPath);

	CU::Vector2<float> windowSize = CU::Vector2<float>(float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y));

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
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));

	if (myMenu->GetMainMenu() == true)
	{
		myStateStack->PushSubGameState(new SplashState("Data/Resource/Texture/Menu/Splash/T_logo_other.dds", myInputWrapper));
		myStateStack->PushSubGameState(new SplashState("Data/Resource/Texture/Menu/Splash/T_logo_our.dds", myInputWrapper));
		myHasFadeIn = true;
	}

	if (myShowVictoryScreen == true)
	{
		myStateStack->PushSubGameState(new SplashState("Data/Resource/Texture/Menu/Splash/T_victory_screen.dds", myInputWrapper));
	}

	myCurrentTime = 0;
	myFadeInTime = 0.5f;
	myOverlayAlpha = 1.f;
	PostMaster::GetInstance()->SendMessage(FadeMessage(1.f / 3.f));
}

void MenuState::EndState()
{
	delete myMenu;
	delete myCamera;
	myMenu = nullptr;
	myCamera = nullptr;
	PostMaster::GetInstance()->SendMessage(FadeMessage(1.f / 3.f));
}

const eStateStatus MenuState::Update(const float& aDeltaTime)
{
	if (myInputWrapper->KeyDown(DIK_ESCAPE) == true)
	{
		return eStateStatus::ePopMainState;
	}

	myCurrentTime += aDeltaTime;
	myOverlayAlpha = fmaxf(1.f - myCurrentTime / myFadeInTime, 0);

	if (myMenu->Update(myInputWrapper) == false)
	{
		return eStateStatus::ePopMainState;
	}

	return eStateStatus::eKeepState;
}

void MenuState::Render()
{
	myMenu->Render(myInputWrapper);

	if (myHasFadeIn == true)
	{
		CU::Vector2<float> windowSize = CU::Vector2<float>(float(Prism::Engine::GetInstance()->GetWindowSize().x),
			float(-Prism::Engine::GetInstance()->GetWindowSize().y));
		myBlackOverlay->Render(windowSize / 2.f, { 1.f, 1.f }, { 1.f, 1.f, 1.f, myOverlayAlpha });
	}
}

void MenuState::ResumeState()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, true));
}

void MenuState::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
	myMenu->OnResize(aWidth, aHeight);
}