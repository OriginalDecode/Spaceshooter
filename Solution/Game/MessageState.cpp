#include "stdafx.h"
#include <Camera.h>
#include <Engine.h>
#include "GameStateMessage.h"
#include "MessageState.h"
#include <Sprite.h>
#include <InputWrapper.h>
#include "PostMaster.h"

MessageState::MessageState(const std::string& aTexturePath, const CU::Vector2<float>& aSize, CU::InputWrapper* anInputWrapper)
	: myEvent(nullptr)
{
	myBackground = new Prism::Sprite(aTexturePath, aSize, aSize/2.f);
	myInputWrapper = anInputWrapper;
	myTextMessage = "";
}

MessageState::~MessageState()
{
	delete myBackground;
	delete myCamera;
	delete myEvent;
	myBackground = nullptr;
	myCamera = nullptr;
	myEvent = nullptr;
}

void MessageState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = true;
	myStateStack = aStateStackProxy;
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
}

void MessageState::EndState()
{
}

const eStateStatus MessageState::Update(const float&)
{
	if (myInputWrapper->KeyDown(DIK_SPACE) == true || myInputWrapper->KeyDown(DIK_ESCAPE) == true)
	{
		if (myEvent != nullptr)
		{
			PostMaster::GetInstance()->SendMessage(*myEvent);
		}
		return eStateStatus::ePopSubState;
	}

	return eStateStatus::eKeepState;
}

void MessageState::Render()
{
	myBackground->Render({ Prism::Engine::GetInstance()->GetWindowSize().x / 2.f, -Prism::Engine::GetInstance()->GetWindowSize().y / 2.f });

	if (myTextMessage != "")
	{
		Prism::Engine::GetInstance()->PrintDebugText(myTextMessage, myMessagePosition);
	}
}

void MessageState::ResumeState()
{

}

void MessageState::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);

	myMessagePosition.x = (Prism::Engine::GetInstance()->GetWindowSize().x / 2.f) - myBackground->GetSize().x / 2;
	myMessagePosition.y = (-Prism::Engine::GetInstance()->GetWindowSize().y / 2.f) + myBackground->GetSize().y / 2;
}