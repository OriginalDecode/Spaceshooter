#pragma once
#include <Matrix.h>
#include <GrowingArray.h>
#include "StateStack.h"
#include "MainMenuState.h"
#include "InGameState.h"

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;

class Game
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

private:
	void operator=(Game& aApp) = delete;

	CU::InputWrapper* myInputWrapper;

	StateStack myStateStack;
	
	MainMenuState* myMainMenu;
	InGameState* myGame;


};