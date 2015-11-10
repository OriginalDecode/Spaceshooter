#include "stdafx.h"
#include <Camera.h>
#include <Engine.h>
#include "FadeMessage.h"
#include "GameStateMessage.h"
#include "MessageState.h"
#include <Sprite.h>
#include <InputWrapper.h>
#include "PostMaster.h"

MessageState::MessageState(const std::string& aTexturePath, const CU::Vector2<float>& aSize, CU::InputWrapper* anInputWrapper
		, const LevelScore& aLevelScore)
	: myEvent(nullptr)
	, myLevelScore(aLevelScore)
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
	PostMaster::GetInstance()->SendMessage(FadeMessage(1.f / 3.f));
}

void MessageState::EndState()
{
	PostMaster::GetInstance()->SendMessage(FadeMessage(1.f / 3.f));
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
		Prism::Engine::GetInstance()->PrintText(myTextMessage, myMessagePosition, Prism::eTextType::RELEASE_TEXT);
	}

	Prism::Engine::GetInstance()->PrintText("Enemies killed: " + std::to_string(myLevelScore.myKilledEnemies) + "/" 
		+ std::to_string(myLevelScore.myTotalEnemies), { myMessagePosition.x, myMessagePosition.y - 200 }
	, Prism::eTextType::RELEASE_TEXT);

	Prism::Engine::GetInstance()->PrintText("Shots fired: "	+ std::to_string(myLevelScore.myTotalShotsFired)
		, { myMessagePosition.x, myMessagePosition.y - 225 }, Prism::eTextType::RELEASE_TEXT);
	Prism::Engine::GetInstance()->PrintText("Shots that hit enemies: " + std::to_string(myLevelScore.myShotsHit)
		, { myMessagePosition.x, myMessagePosition.y - 250 }, Prism::eTextType::RELEASE_TEXT);
	Prism::Engine::GetInstance()->PrintText("Hit rate: " 
		+ std::to_string(static_cast<float>(myLevelScore.myShotsHit) / static_cast<float>(myLevelScore.myTotalShotsFired) * 100.f) + "%"
		, { myMessagePosition.x, myMessagePosition.y - 275 }, Prism::eTextType::RELEASE_TEXT);

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