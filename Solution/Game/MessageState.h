#pragma once
#include "GameState.h"
#include "LevelScore.h"

namespace Prism
{
	class Sprite;
	class Camera;
}
struct LevelScore;
class GameStateMessage;

class MessageState : public GameState
{
public:
	MessageState(const std::string& aTexturePath, const CU::Vector2<float>& aSize, CU::InputWrapper* anInputWrapper
		, const LevelScore& aLevelScore);
	~MessageState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update(const float& aDeltaTime) override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

	void SetText(const std::string& aText);
	void SetEvent(GameStateMessage* anEvent);

private:
	void RenderBadgesAndStars(const CU::Vector2<float>& aRenderPos);

	Prism::Camera* myCamera;
	Prism::Sprite* myBackground;
	Prism::Sprite* myOptionalBadgeGrey;
	Prism::Sprite* myOptionalBadge;
	Prism::Sprite* myStarGrey;
	Prism::Sprite* myStar;


	std::string myTextMessage;
	CU::Vector2<float> myMessagePosition;
	GameStateMessage* myEvent;

	LevelScore myLevelScore;
	SaveScore mySaveScore;

	bool myShowBadge;
};

inline void MessageState::SetText(const std::string& aText)
{
	myTextMessage = aText;
}

inline void MessageState::SetEvent(GameStateMessage* anEvent)
{
	myShowBadge = myEvent == nullptr || myEvent->GetGameState() != eGameState::RELOAD_LEVEL;
	myEvent = anEvent;
}
