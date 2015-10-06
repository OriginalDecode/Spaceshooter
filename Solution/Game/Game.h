#pragma once
#include <Matrix.h>
#include <GrowingArray.h>
#include "StateStack.h"
#include "Subscriber.h"

class MenuState;
class InGameState;

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;

class Game : public Subscriber
{
public:
	Game();
	~Game();

	bool Init(HWND& aHwnd);
	bool Destroy();
	bool Update();

	void Pause();
	void UnPause();
	void OnResize(int aWidth, int aHeight);

	void ReceiveMessage(const GameStateMessage& aMessage) override;

private:
	void operator=(Game& aApp) = delete;

	CU::InputWrapper* myInputWrapper;

	StateStack myStateStack;
	
	MenuState* myMainMenu;
	MenuState* myLevelSelect;
	InGameState* myGame;

	bool myLockMouse;

	CU::Vector2<int> myWindowSize;

};