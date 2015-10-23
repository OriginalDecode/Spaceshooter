#pragma once
#include "GameState.h"
#include <Matrix.h>
#include <GrowingArray.h>
#include "Subscriber.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;
class CollisionManager;
class LevelFactory;
class Level;
class Entity;
class MessageState;

class InGameState : public GameState, public Subscriber
{
public:

	InGameState(CU::InputWrapper* anInputWrapper, const bool& aShowMessages = true);
	~InGameState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update(const float& aDeltaTime) override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

	void ReceiveMessage(const GameStateMessage& aMessage) override;

	void SetLevel(int aLevelID);

	void CompleteLevel();
	void CompleteGame();
	void LoadLevelSettings(); 
	void LoadPlayerSettings();

private:
	void ShowMessage(const std::string& aBackgroundPath, const CU::Vector2<float>& aSize, std::string aText, GameStateMessage* aMessage = nullptr);
	
	LevelFactory* myLevelFactory;
	Level* myLevel;
	Entity* myPlayer;

	MessageState* myMessageScreen;

	bool myIsComplete;
	bool myShowMessages;
};