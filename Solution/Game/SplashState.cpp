#include "stdafx.h"
#include "SplashState.h"
#include <Sprite.h>
#include <Camera.h>
#include <InputWrapper.h>
#include "PostMaster.h"
#include "StateStackProxy.h"
#include <TimerManager.h>
#include "GameStateMessage.h"
#include "Menu.h"
#include "Button.h"

SplashState::SplashState(const std::string& aTexturePath, CU::InputWrapper* anInputWrapper)
{
	CU::Vector2<float> size(1024.f, 1024.f);
	myLogo = new Prism::Sprite(aTexturePath, size, size / 2.f);

	CU::Vector2<float> windowSize = CU::Vector2<float>(Prism::Engine::GetInstance()->GetWindowSize().x,
		Prism::Engine::GetInstance()->GetWindowSize().y);

	CU::Vector2<float> backgroundSize(4096.f, 4096.f);
	myBackground = new Prism::Sprite("Data/Resource/Texture/Menu/Splash/T_background_default.dds", windowSize, windowSize / 2.f);
	myInputWrapper = anInputWrapper;
}

SplashState::~SplashState()
{
}

void SplashState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = false;
	myStateStack = aStateStackProxy;
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, false));
	
	myLogoAlpha = 0.f;

	myFadeInTime = 1.5f;
	myFadeOutTime = 1.5f;
	myDisplayTime = 1.5f;
	myCurrentTime = 0;
	myFadeOut = false;
	myDisplay = false;
}

void SplashState::EndState()
{
	delete myLogo;
	delete myBackground;
}

const eStateStatus SplashState::Update(const float&)
{
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	deltaTime = fminf(1.f / 30.f, deltaTime);

	myCurrentTime += deltaTime;

	if (myInputWrapper->KeyDown(DIK_SPACE) == true
		|| myInputWrapper->KeyDown(DIK_ESCAPE) == true
		|| myInputWrapper->KeyDown(DIK_RETURN) == true
		|| myInputWrapper->MouseDown(0) == true
		|| myInputWrapper->MouseDown(1) == true
		|| myInputWrapper->MouseDown(2) == true)
	{
		myCurrentTime += myFadeInTime + myDisplayTime;
	}

	if (myFadeOut == true)
	{
		myLogoAlpha -= deltaTime / myFadeOutTime;
	}
	else if (myDisplay == false)
	{
		myLogoAlpha += deltaTime / myFadeInTime;
	}
	else
	{
		myLogoAlpha = 1.f;
	}
	
	myLogoAlpha = fmaxf(fminf(myLogoAlpha, 1.f), 0);

	if (myDisplay == false && myCurrentTime > myFadeInTime)
	{
		myDisplay = true;
	}
	if (myFadeOut == false && myCurrentTime > myFadeInTime + myDisplayTime)
	{
		myFadeOut = true;
	}

	if (myFadeOut == true && myLogoAlpha == 0)
	{
		return ePopSubState;
	}

	return eKeepState;
}

void SplashState::Render()
{
	CU::Vector2<float> windowSize = CU::Vector2<float>(Prism::Engine::GetInstance()->GetWindowSize().x,
		-Prism::Engine::GetInstance()->GetWindowSize().y);
	myBackground->Render(windowSize / 2.f);
	myLogo->Render(windowSize / 2.f, { 1.f, 1.f }, { 1.f, 1.f, 1.f, myLogoAlpha});
}

void SplashState::ResumeState()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::MOUSE_LOCK, false));
}

void SplashState::OnResize(int aWidth, int aHeight)
{
	if (myBackground != nullptr)
	{
		myBackground->SetSize({ float(aWidth), float(aHeight) });
	}
	myCamera->OnResize(aWidth, aHeight);
}