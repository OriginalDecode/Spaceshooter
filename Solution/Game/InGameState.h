#pragma once
#include "GameState.h"
#include <Matrix.h>
#include <GrowingArray.h>

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;
class CollisionManager;
class Level;
class MessageState;

class InGameState : public GameState
{
public:

	InGameState(CU::InputWrapper* anInputWrapper, const bool& aShowMessages = true);
	~InGameState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

	void SetLevel(Level* aLevel);

	void CompleteLevel();
	void CompleteGame();

private:
	void ShowMessage(const std::string& aBackgroundPath, const CU::Vector2<float>& aSize, std::string aText, GameStateMessage* aMessage = nullptr);
	
	Level* myLevel;

	MessageState* myMessageScreen;

	bool myIsComplete;
	bool myShowMessages;
};

inline void InGameState::SetLevel(Level* aLevel)
{
	myLevel = aLevel;
}