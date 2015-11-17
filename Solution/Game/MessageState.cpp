#include "stdafx.h"
#include <AudioInterface.h>
#include <Camera.h>
#include <Engine.h>
#include "FadeMessage.h"
#include "GameStateMessage.h"
#include "MessageState.h"
#include "ScoreIO.h"
#include <Sprite.h>
#include <InputWrapper.h>
#include "PostMaster.h"

MessageState::MessageState(const std::string& aTexturePath, const CU::Vector2<float>& aSize, CU::InputWrapper* anInputWrapper
	, const LevelScore& aLevelScore)
	: myEvent(nullptr)
	, myLevelScore(aLevelScore)
{
	DL_ASSERT_EXP(aLevelScore.myLevel >= 0, "Invalid level number.");

	myShowBadge = myEvent == nullptr || myEvent->GetGameState() != eGameState::RELOAD_LEVEL;
	myBackground = new Prism::Sprite(aTexturePath, aSize, aSize/2.f);
	myInputWrapper = anInputWrapper;
	myTextMessage = "";
	mySpriteSize = {64.f, 64.f};
	myOptionalBadgeGrey = new Prism::Sprite("Data/Resource/Texture/Menu/BadgeGrey.dds", mySpriteSize, mySpriteSize * 0.5f);
	myOptionalBadge = new Prism::Sprite("Data/Resource/Texture/Menu/Badge.dds", mySpriteSize, mySpriteSize * 0.5f);
	myStarGrey = new Prism::Sprite("Data/Resource/Texture/Menu/StarGrey.dds", mySpriteSize, mySpriteSize * 0.5f);
	myBronzeStar = new Prism::Sprite("Data/Resource/Texture/Menu/StarBronze.dds", mySpriteSize, mySpriteSize * 0.5f);
	mySilverStar = new Prism::Sprite("Data/Resource/Texture/Menu/StarSilver.dds", mySpriteSize, mySpriteSize * 0.5f);
	myGoldStar = new Prism::Sprite("Data/Resource/Texture/Menu/StarGold.dds", mySpriteSize, mySpriteSize * 0.5f);

	CU::Vector2<float> windowSize = CU::Vector2<float>(float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y));

	myBlackOverlay = new Prism::Sprite("Data/Resource/Texture/Menu/Splash/T_background_default.dds", windowSize, windowSize / 2.f);

	float oneStarLimit = myLevelScore.myTotalEnemies * 0.33f;
	float twoStarLimit = myLevelScore.myTotalEnemies * 0.66f;
	float threeStarLimit = myLevelScore.myTotalEnemies * 0.80f;

	mySaveScore.myStars = 0;
	if (myLevelScore.myKilledEnemies >= oneStarLimit && myLevelScore.myKilledEnemies < twoStarLimit)
	{
		mySaveScore.myStars = 1;
	}
	else if (myLevelScore.myKilledEnemies >= twoStarLimit && myLevelScore.myKilledEnemies < threeStarLimit)
	{
		mySaveScore.myStars = 2;
	}
	else if (myLevelScore.myKilledEnemies >= threeStarLimit)
	{
		mySaveScore.myStars = 3;
	}

	mySaveScore.myCompletedOptional = myLevelScore.myCompletedOptional;
	mySaveScore.myTotalOptional = myLevelScore.myTotalOptional;
	mySaveScore.myDifficulty = myLevelScore.myDifficulty;
}

MessageState::~MessageState()
{
	delete myBackground;
	delete myCamera;
	delete myEvent;
	delete myOptionalBadge;
	delete myOptionalBadgeGrey;
	delete myStarGrey;
	delete myBronzeStar;
	delete mySilverStar;
	delete myGoldStar;
	delete myBlackOverlay;
	myBackground = nullptr;
	myCamera = nullptr;
	myEvent = nullptr;
	myOptionalBadge = nullptr;
	myOptionalBadgeGrey = nullptr;
	myStarGrey = nullptr;
	myBronzeStar = nullptr;
	mySilverStar = nullptr;
	myGoldStar = nullptr;
	myBlackOverlay = nullptr;
}

void MessageState::InitState(StateStackProxy* aStateStackProxy)
{
	myIsLetThrough = true;
	myStateStack = aStateStackProxy;
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
	PostMaster::GetInstance()->SendMessage(FadeMessage(1.f / 3.f));

	if (myShowBadge)
	{
		ScoreIO::Save(mySaveScore, myLevelScore.myLevel);
	}
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Mute", 0);
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
	CU::Vector2<float> windowSize = CU::Vector2<float>(float(Prism::Engine::GetInstance()->GetWindowSize().x / 2.f)
		, float(-Prism::Engine::GetInstance()->GetWindowSize().y / 2.f));

	myBlackOverlay->Render(windowSize, { 1.f, 1.f }, { 1.f, 1.f, 1.f, 0.5f });

	float leftMargin = 50.f;
	float rowHeight = 28.f;
	float offsetY = -480.f;

	myBackground->Render(windowSize);

	if (myTextMessage != "")
	{
		Prism::Engine::GetInstance()->PrintText(myTextMessage, { myMessagePosition.x + 342.f, myMessagePosition.y - 740.f }, Prism::eTextType::RELEASE_TEXT);
	}

	Prism::Engine::GetInstance()->PrintText("Enemies killed: " + std::to_string(myLevelScore.myKilledEnemies) + " / " 
		+ std::to_string(myLevelScore.myTotalEnemies), { myMessagePosition.x + leftMargin, myMessagePosition.y + offsetY }
	, Prism::eTextType::RELEASE_TEXT);

	Prism::Engine::GetInstance()->PrintText("Shots fired: "	+ std::to_string(myLevelScore.myTotalShotsFired)
		, { myMessagePosition.x + leftMargin, myMessagePosition.y + offsetY - rowHeight }, Prism::eTextType::RELEASE_TEXT);

	float denom = fmaxf(float(myLevelScore.myTotalShotsFired), 1.f);
	Prism::Engine::GetInstance()->PrintText("Hit rate: " 
		+ std::to_string(int(float((myLevelScore.myShotsHit) / denom * 100.f))) + "%"
		, { myMessagePosition.x + leftMargin, myMessagePosition.y + offsetY - rowHeight * 2.f }, Prism::eTextType::RELEASE_TEXT);

	Prism::Engine::GetInstance()->PrintText("Optional missions completed: "
		, { myMessagePosition.x + leftMargin, myMessagePosition.y + offsetY - rowHeight * 3.f }, Prism::eTextType::RELEASE_TEXT);

	if (myShowBadge)
	{
		RenderBadgesAndStars(myMessagePosition);
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

void MessageState::RenderBadgesAndStars(const CU::Vector2<float>& aRenderPos)
{
	float starOffsetY = 372.f;
	float badgesOffsetY = 564.f;
	for (int i = 0; i < 3; ++i)
	{
		if (i < mySaveScore.myStars)
		{
			if (mySaveScore.myDifficulty == 0)
			{
				myBronzeStar->Render({ (Prism::Engine::GetInstance()->GetWindowSize().x / 2.f) + (64.f * (i - 1)), aRenderPos.y - starOffsetY });
			}
			else if (mySaveScore.myDifficulty == 1)
			{
				mySilverStar->Render({ (Prism::Engine::GetInstance()->GetWindowSize().x / 2.f) + (64.f * (i - 1)), aRenderPos.y - starOffsetY });
			}
			else if (mySaveScore.myDifficulty == 2)
			{
				myGoldStar->Render({ (Prism::Engine::GetInstance()->GetWindowSize().x / 2.f) + (64.f * (i - 1)), aRenderPos.y - starOffsetY });
			}
		}
		else
		{
			myStarGrey->Render({ (Prism::Engine::GetInstance()->GetWindowSize().x / 2.f) + (64.f * (i - 1)), aRenderPos.y - starOffsetY });
		}
	}

	for (int i = 1; i <= mySaveScore.myTotalOptional; ++i)
	{
		if (mySaveScore.myCompletedOptional < i)
		{
			myOptionalBadgeGrey->Render({ myMessagePosition.x + 400.f + (mySpriteSize.x * i), myMessagePosition.y - badgesOffsetY });
		}
		else
		{
			myOptionalBadge->Render({ myMessagePosition.x + 400.f + (mySpriteSize.x * i), myMessagePosition.y - badgesOffsetY });
		}
	}
}