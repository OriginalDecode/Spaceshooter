#pragma once
#include "GameState.h"

namespace Prism
{
	class Model2D;
	class Camera;
}

class GameStateMessage;

class MessageState : public GameState
{
public:
	MessageState(const std::string& aTexturePath, const CU::Vector2<float>& aSize, CU::InputWrapper* anInputWrapper);
	~MessageState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

	void SetText(const std::string& aText);
	void SetEvent(GameStateMessage* anEvent);

private:

	Prism::Camera* myCamera;
	Prism::Model2D* myBackground;

	std::string myTextMessage;
	CU::Vector2<float> myMessagePosition;
	GameStateMessage* myEvent;
};

inline void MessageState::SetText(const std::string& aText)
{
	myTextMessage = aText;
}

inline void MessageState::SetEvent(GameStateMessage* anEvent)
{
	myEvent = anEvent;
}
